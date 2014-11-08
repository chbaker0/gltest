#include "ShaderProgram.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

bool ShaderProgram::link(std::string *log)
{
	glLinkProgram(handle);

	GLint status;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);

	if(log)
	{
		GLint len;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &len);
		log->resize(len+1);
		glGetProgramInfoLog(handle, len, nullptr, &(*log)[0]);
		log->pop_back();
	}

	return status;
}

template <>
void ShaderProgram::setUniform<float>(unsigned int loc, const float& value) const
{
    glProgramUniform1f(handle, loc, value);
}
template <>
void ShaderProgram::setUniform<glm::vec2>(unsigned int loc, const glm::vec2& value) const
{
    glProgramUniform2fv(handle, loc, 1, glm::value_ptr(value));
}
template <>
void ShaderProgram::setUniform<glm::vec3>(unsigned int loc, const glm::vec3& value) const
{
    glProgramUniform3fv(handle, loc, 1, glm::value_ptr(value));
}
template <>
void ShaderProgram::setUniform<glm::vec4>(unsigned int loc, const glm::vec4& value) const
{
    glProgramUniform4fv(handle, loc, 1, glm::value_ptr(value));
}
template <>
void ShaderProgram::setUniform<glm::mat2>(unsigned int loc, const glm::mat2& value) const
{
    glProgramUniformMatrix2fv(handle, loc, 1, GL_FALSE, glm::value_ptr(value));
}
template <>
void ShaderProgram::setUniform<glm::mat3>(unsigned int loc, const glm::mat3& value) const
{
    glProgramUniformMatrix3fv(handle, loc, 1, GL_FALSE, glm::value_ptr(value));
}
template <>
void ShaderProgram::setUniform<glm::mat4>(unsigned int loc, const glm::mat4& value) const
{
    glProgramUniformMatrix4fv(handle, loc, 1, GL_FALSE, glm::value_ptr(value));
}
