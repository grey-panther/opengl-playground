#include "ShaderUniformStore.hpp"

#include "ShaderUniform.hpp"
#include "Utilities.hpp"


ShaderUniformStore::ShaderUniformStore() = default;

ShaderUniformStore::~ShaderUniformStore() = default;


void ShaderUniformStore::resetAll()
{
	_uniforms.clear();
}


void ShaderUniformStore::reset(std::string_view name)
{
	_uniforms.erase(std::string(name));
}


void ShaderUniformStore::set(ShaderUniformPtr&& uniformPtr)
{
	if (!uniformPtr) {
		return;
	}
	_uniforms[uniformPtr->getName()] = std::move(uniformPtr);
}
