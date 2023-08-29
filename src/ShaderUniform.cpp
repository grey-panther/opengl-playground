#include "ShaderUniform.hpp"
#include "Utilities.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glad/glad.h>


class UniformSampler : public ShaderUniform
{
public:
	UniformSampler(const std::string_view name, int value)
			: ShaderUniform(name, 1)
			, _value(value)
	{
	}

	void bind(int uniformLocation) const override
	{
		glUniform1i(uniformLocation, _value);
	}

private:
	int _value = 0;
};


class UniformInt : public ShaderUniform
{
public:
	UniformInt(const std::string_view name, int value)
			: ShaderUniform(name, 1)
			, _value(value)
	{
	}

	void bind(int uniformLocation) const override
	{
		glUniform1i(uniformLocation, _value);
	}

private:
	int _value = 0;
};


class UniformFloat : public ShaderUniform
{
public:
	UniformFloat(const std::string_view name, float value)
			: ShaderUniform(name, 1)
			, _value(value)
	{
	}

	void bind(int uniformLocation) const override
	{
		glUniform1f(uniformLocation, _value);
	}

private:
	float _value = 0.f;
};


class UniformVec2 : public ShaderUniform
{
public:
	UniformVec2(const std::string_view name, float v1, float v2)
			: ShaderUniform(name, 1)
			, _v1(v1)
			, _v2(v2)
	{
	}

	void bind(int uniformLocation) const override
	{
		glUniform2f(uniformLocation, _v1, _v2);
	}

private:
	float _v1 = 0.f;
	float _v2 = 0.f;
};


class UniformVec3 : public ShaderUniform
{
public:
	UniformVec3(const std::string_view name, float v1, float v2, float v3)
			: ShaderUniform(name, 1)
			, _v1(v1)
			, _v2(v2)
			, _v3(v3)
	{
	}

	void bind(int uniformLocation) const override
	{
		glUniform3f(uniformLocation, _v1, _v2, _v3);
	}

private:
	float _v1 = 0.f;
	float _v2 = 0.f;
	float _v3 = 0.f;
};


class UniformVec4 : public ShaderUniform
{
public:
	UniformVec4(const std::string_view name, float v1, float v2, float v3, float v4)
			: ShaderUniform(name, 1)
			, _v1(v1)
			, _v2(v2)
			, _v3(v3)
			, _v4(v4)
	{
	}

	void bind(int uniformLocation) const override
	{
		glUniform4f(uniformLocation, _v1, _v2, _v3, _v4);
	}

private:
	float _v1 = 0.f;
	float _v2 = 0.f;
	float _v3 = 0.f;
	float _v4 = 0.f;
};


class UniformMat4 : public ShaderUniform
{
public:
	UniformMat4(const std::string_view name, const glm::mat4& mat)
			: ShaderUniform(name, 1)
			, _mat(mat)
	{
	}

	void bind(int uniformLocation) const override
	{
		const GLboolean transpose = GL_FALSE;
		glUniformMatrix4fv(uniformLocation, getArraySize(), transpose, glm::value_ptr(_mat));
	}

private:
	glm::mat4 _mat;
};


ShaderUniform::ShaderUniform(const std::string_view name, int arraySize)
		: _name(name)
		, _arraySize(arraySize)
{
	assertFalse(_name.empty());
	_arraySize = std::max(_arraySize, 1);
}


ShaderUniformPtr ShaderUniform::Sampler(const std::string_view name, int value)
{
	return std::make_unique<UniformSampler>(name, value);
}


ShaderUniformPtr ShaderUniform::Float(const std::string_view name, float value)
{
	return std::make_unique<UniformFloat>(name, value);
}


ShaderUniformPtr ShaderUniform::Int(const std::string_view name, int value)
{
	return std::make_unique<UniformInt>(name, value);
}


ShaderUniformPtr ShaderUniform::Vec2(const std::string_view name, float v1, float v2)
{
	return std::make_unique<UniformVec2>(name, v1, v2);
}


ShaderUniformPtr ShaderUniform::Vec2(const std::string_view name, const glm::vec2& vec)
{
	return std::make_unique<UniformVec2>(name, vec.x, vec.y);
}


ShaderUniformPtr ShaderUniform::Vec3(const std::string_view name, float v1, float v2, float v3)
{
	return std::make_unique<UniformVec3>(name, v1, v2, v3);
}


ShaderUniformPtr ShaderUniform::Vec3(const std::string_view name, const glm::vec3& vec)
{
	return std::make_unique<UniformVec3>(name, vec.x, vec.y, vec.z);
}


ShaderUniformPtr ShaderUniform::Vec4(const std::string_view name, float v1, float v2, float v3, float v4)
{
	return std::make_unique<UniformVec4>(name, v1, v2, v3, v4);
}


ShaderUniformPtr ShaderUniform::Vec4(const std::string_view name, const glm::vec4& vec)
{
	return std::make_unique<UniformVec4>(name, vec.x, vec.y, vec.z, vec.w);
}


ShaderUniformPtr ShaderUniform::Mat4(const std::string_view name, const glm::mat4& mat)
{
	return std::make_unique<UniformMat4>(name, mat);
}
