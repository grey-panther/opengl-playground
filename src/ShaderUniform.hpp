#pragma once

#include <glm/fwd.hpp>

#include <memory>
#include <string>
#include <string_view>


class ShaderUniform;
using ShaderUniformPtr = std::unique_ptr<ShaderUniform>;

/**
 * @brief Represents one named uniform variable in a shader program.
 */
class ShaderUniform
{
public:
	ShaderUniform(std::string_view name, int arraySize);

	virtual ~ShaderUniform() noexcept = default;

	[[nodiscard]]
	const std::string& getName() const { return _name; }

	[[nodiscard]]
	int getArraySize() const { return _arraySize; }

	virtual void bind(int uniformLocation) const = 0;

	[[nodiscard]]
	static ShaderUniformPtr Sampler(std::string_view name, int sampler);

	[[nodiscard]]
	static ShaderUniformPtr Int(std::string_view name, int value);

	[[nodiscard]]
	static ShaderUniformPtr Float(std::string_view name, float value);

	[[nodiscard]]
	static ShaderUniformPtr Vec2(std::string_view name, float v1, float v2);

	[[nodiscard]]
	static ShaderUniformPtr Vec2(std::string_view name, const glm::vec2& vec);

	[[nodiscard]]
	static ShaderUniformPtr Vec3(std::string_view name, float v1, float v2, float v3);

	[[nodiscard]]
	static ShaderUniformPtr Vec3(std::string_view name, const glm::vec3& vec);

	[[nodiscard]]
	static ShaderUniformPtr Vec4(std::string_view name, float v1, float v2, float v3, float v4);

	[[nodiscard]]
	static ShaderUniformPtr Vec4(std::string_view name, const glm::vec4& vec);

	[[nodiscard]]
	static ShaderUniformPtr Mat4(std::string_view name, const glm::mat4& mat);

private:
	// The name of the uniform variable in a shader.
	std::string _name;
	// The size of the uniform array denoted by _name. It must be 1, if the uniform isn't an array.
	int _arraySize = 0;
};
