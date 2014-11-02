#include "ShaderProgram.hpp"

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
