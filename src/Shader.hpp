#pragma once

#include <string>
#include <string_view>

#include <glad/glad.h>


class Shader
{
	enum class ShaderType {
		Vertex,
		Fragment,
	};

public:
	Shader(std::string_view vertexFileName, std::string_view fragmentFileName);

	~Shader() noexcept;

	Shader(const Shader&) = delete;

	Shader& operator=(const Shader&) = delete;

	Shader(Shader&& other) noexcept;

	Shader& operator=(Shader&& other) noexcept;

	[[nodiscard]]
	bool isValid() const { return glIsProgram(_shaderProgramId); };

	void bind() const;

	void setUniform1f(const std::string& name, float v1) const;

	void setUniform2f(const std::string& name, float v1, float v2) const;

	void setUniform3f(const std::string& name, float v1, float v2, float v3) const;

	void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4) const;

private:
	static std::string loadShaderText(std::string_view fileName);

	static GLuint compileShader(ShaderType type, const std::string& sourceText);

	static GLuint linkProgram(GLuint vertexShaderId, GLuint fragmentShaderId);

	static GLenum getGlShaderType(ShaderType type);

	void deleteProgram();

private:
	GLuint _shaderProgramId = 0;
};
