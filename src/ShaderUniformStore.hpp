#pragma once

#include <memory>
#include <unordered_map>


class Shader;
class ShaderUniform;
using ShaderUniformPtr = std::unique_ptr<ShaderUniform>;

/**
 * @brief Represents a store for uniform variables of one particular shader program.
 */
class ShaderUniformStore
{
public:
	ShaderUniformStore();

	~ShaderUniformStore();

	void resetAll();

	void reset(std::string_view name);

	void set(ShaderUniformPtr&& uniformPtr);

	auto begin() const { return _uniforms.begin(); };

	auto end() const { return _uniforms.end(); };

private:
	std::unordered_map<std::string, ShaderUniformPtr> _uniforms;
};
