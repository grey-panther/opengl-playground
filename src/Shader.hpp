#pragma once

#include <memory>
#include <string>
#include <string_view>

#include <glad/glad.h>
#include <glm/fwd.hpp>


class ShaderUniformStore;
using ShaderUniformStorePtr = std::unique_ptr<ShaderUniformStore>;

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

	ShaderUniformStore& getUniforms() { return *_uniforms; }

	const ShaderUniformStore& getUniforms() const { return *_uniforms; }

private:
	void updateUniformValues() const;

	static std::string loadShaderText(std::string_view fileName);

	static GLuint compileShader(ShaderType type, const std::string& sourceText);

	static GLuint linkProgram(GLuint vertexShaderId, GLuint fragmentShaderId);

	static GLenum getGlShaderType(ShaderType type);

	void deleteProgram();

private:
	GLuint _shaderProgramId = 0;

	ShaderUniformStorePtr _uniforms;
};
