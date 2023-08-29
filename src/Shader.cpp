#include "Shader.hpp"

#include "Utilities.hpp"
#include "ShaderUniform.hpp"
#include "ShaderUniformStore.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>


Shader::Shader(std::string_view vertexFileName, std::string_view fragmentFileName)
{
	_uniforms = std::make_unique<ShaderUniformStore>();

	// Load and compile vertex shader.
	const std::string vertexText = loadShaderText(vertexFileName);
	const GLuint vertexShaderId = compileShader(ShaderType::Vertex, vertexText);
	if (vertexShaderId == 0) {
		std::cerr << "[Shader] Shader is not compiled: " << vertexFileName << std::endl;
	}

	// Load and compile fragment shader.
	const std::string fragmentText = loadShaderText(fragmentFileName);
	const GLuint fragmentShaderId = compileShader(ShaderType::Fragment, fragmentText);
	if (fragmentShaderId == 0) {
		std::cerr << "[Shader] Shader is not compiled: " << fragmentFileName << std::endl;
	}

	// Create and link shader program.
	_shaderProgramId = linkProgram(vertexShaderId, fragmentShaderId);
	if (_shaderProgramId > 0) {
		std::cout << "[Shader] Shader is loaded.\n"
				<< "\tvertex: " << vertexFileName << "\n"
				<< "\tfragment: " << fragmentFileName
				<< std::endl;
	}

	// When the shader program is linked, shaders are no longer needed.
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}


Shader::~Shader() noexcept
{
	deleteProgram();
}


Shader::Shader(Shader&& other) noexcept
{
	*this = std::move(other);
}


Shader& Shader::operator=(Shader&& other) noexcept
{
	if (&other == this) {
		return *this;
	}

	deleteProgram();
	std::swap(_shaderProgramId, other._shaderProgramId);
	std::swap(_uniforms, other._uniforms);
	return *this;
}


void Shader::deleteProgram()
{
	if (isValid()) {
		glDeleteProgram(_shaderProgramId);
	}
	_shaderProgramId = 0;
	_uniforms->resetAll();
}


std::string Shader::loadShaderText(std::string_view fileName)
{
	std::string result;

	auto inputFile = std::ifstream(std::string(fileName));
	if (!inputFile) {
		std::cerr << "[Shader] File opening failed: " << fileName << std::endl;
		return result;
	}

	std::stringstream ss;
	ss << inputFile.rdbuf();
	result = ss.str();

	return result;
}


GLuint Shader::compileShader(Shader::ShaderType type, const std::string& sourceText)
{
	assertTrue(!sourceText.empty());
	if (sourceText.empty()) {
		return 0;
	}

	const GLenum shaderType = getGlShaderType(type);
	const GLuint shaderId = glCreateShader(shaderType);

	const char* srcTextPtr = sourceText.data();
	glShaderSource(shaderId, 1, &srcTextPtr, nullptr);
	glCompileShader(shaderId);

	// Check for errors.
	GLint status = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		std::string msg;
		msg.resize(512);
		glGetShaderInfoLog(shaderId, (GLint) msg.size(), nullptr, msg.data());
		std::string typeStr = (type == ShaderType::Vertex) ? "Vertex" : "Fragment";
		std::cerr << "[Shader] " << typeStr << " shader compilation failed:\n\t" << msg << std::endl;

		glDeleteShader(shaderId);
		return 0;
	}

	return shaderId;
}


GLuint Shader::linkProgram(GLuint vertexShaderId, GLuint fragmentShaderId)
{
	assertTrue(vertexShaderId > 0);
	assertTrue(fragmentShaderId > 0);

	// Create and link shader program.
	const GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	// Check for errors.
	GLint status = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		std::string msg;
		msg.resize(512);
		glGetProgramInfoLog(programId, (GLint) msg.size(), nullptr, msg.data());
		std::cerr << "Shader program link failed:\n\t" << msg << std::endl;

		glDeleteProgram(programId);
		return 0;
	}

	return programId;
}


void Shader::bind() const
{
	assertTrue(isValid());
	glUseProgram(_shaderProgramId);

	updateUniformValues();
}


void Shader::updateUniformValues() const
{
	assertTrue(isValid());
	// Also assert that the current program is bound to GL context. Don't know yet how to check it.

	if (!_uniforms) {
		return;
	}

	for (const auto& [name, uniformPtr] : *_uniforms) {
		if (!uniformPtr) {
			return;
		}

		// TODO Check that the type of *uniformPtr matches
		//  to the type of the real uniform in the shader program.

		// glGetUniformLocation must be called when the program is bound.
		const GLint location = glGetUniformLocation(_shaderProgramId, name.data());
		if (location >= 0) {
			uniformPtr->bind(location);

			handleGLErrors();
		}
	}
}


GLenum Shader::getGlShaderType(Shader::ShaderType type)
{
	GLenum shaderType = GL_INVALID_ENUM;
	switch (type) {
		case ShaderType::Vertex:
			shaderType = GL_VERTEX_SHADER;
			break;
		case ShaderType::Fragment:
			shaderType = GL_FRAGMENT_SHADER;
			break;
		default:
			assertTrueMsg(false, "Unknown Shader::ShaderType: " + std::to_string(static_cast<int>(type)));
			break;
	}

	return shaderType;
}
