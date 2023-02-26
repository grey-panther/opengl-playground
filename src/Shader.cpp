#include "Shader.hpp"
#include "Utilities.hpp"

#include <fstream>
#include <iostream>
#include <sstream>


Shader::Shader(std::string_view vertexFileName, std::string_view fragmentFileName)
{
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
	return *this;
}


void Shader::deleteProgram()
{
	if (isValid()) {
		glDeleteProgram(_shaderProgramId);
	}
	_shaderProgramId = 0;
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
	assert(!sourceText.empty());
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
	assert(vertexShaderId > 0);
	assert(fragmentShaderId > 0);

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
	assert(isValid());
	glUseProgram(_shaderProgramId);
}


void Shader::setUniform1f(const std::string& name, float v1) const
{
	assert(isValid());

	// Must be called when the program is bound.
	const GLint location = glGetUniformLocation(_shaderProgramId, name.data());
	if (location >= 0) {
		glUniform1f(location, v1);
	}
}


void Shader::setUniform2f(const std::string& name, float v1, float v2) const
{
	assert(isValid());

	// Must be called when the program is bound.
	const GLint location = glGetUniformLocation(_shaderProgramId, name.data());
	if (location >= 0) {
		glUniform2f(location, v1, v2);
	}
}


void Shader::setUniform3f(const std::string& name, float v1, float v2, float v3) const
{
	assert(isValid());

	// Must be called when the program is bound.
	const GLint location = glGetUniformLocation(_shaderProgramId, name.data());
	if (location >= 0) {
		glUniform3f(location, v1, v2, v3);
	}
}


void Shader::setUniform4f(const std::string& name, float v1, float v2, float v3, float v4) const
{
	assert(isValid());

	// Must be called when the program is bound.
	const GLint location = glGetUniformLocation(_shaderProgramId, name.data());
	if (location >= 0) {
		glUniform4f(location, v1, v2, v3, v4);
	}
}


void Shader::setUniform1i(const std::string& name, int v1) const
{
	assert(isValid());

	// Must be called when the program is bound.
	const GLint location = glGetUniformLocation(_shaderProgramId, name.data());
	if (location >= 0) {
		glUniform1i(location, v1);
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
			assert2(false, "Unknown Shader::ShaderType: " + std::to_string(static_cast<int>(type)));
			break;
	}

	return shaderType;
}
