#ifndef SHADER_PROGRAM_HPP_INCLUDED
#define SHADER_PROGRAM_HPP_INCLUDED

#include "Includes.h"
#include "Shader.hpp"

class ShaderProgram
{
public:
	ShaderProgram(): handle(glCreateProgram()) {}
	~ShaderProgram()
	{
		glDeleteProgram(handle);
	}

	void attachShader(const Shader& s)
	{
		glAttachShader(handle, s.getHandle());
	}
	void detachShader(const Shader& s)
	{
		glDetachShader(handle, s.getHandle());
	}

	bool link(std::string *log);

	GLuint getHandle() const
	{
		return handle;
	}

	void use() const
	{
		glUseProgram(handle);
	}

	static void stop()
	{
		glUseProgram(0);
	}

private:
	GLuint handle;

	friend class ShaderProgramManager;
};

#endif // GLSHADERPROGRAM_H
