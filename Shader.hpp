#ifndef SHADER_HPP_INCLUDED
#define SHADER_HPP_INCLUDED

#include "Includes.h"

class Shader
{
public:
	Shader(): handle(0) {}
    Shader(GLenum type): handle(glCreateShader(type)) {}
	Shader(GLenum type, std::ifstream& source_file): Shader(type)
	{
		loadSource(source_file);
	}
	Shader(GLenum type, std::ifstream&& source_file): Shader(type, source_file) {}
	Shader(GLenum type, const std::string& source): Shader(type)
	{
		loadSource(source);
	}
	~Shader()
	{
		glDeleteShader(handle);
	}

	void create(GLenum type)
	{
		handle = glCreateShader(type);
	}

	void loadSource(const std::string& source);
	void loadSource(std::ifstream& source_file);
	void loadSource(std::ifstream&& source_file);

	bool compile(std::string *log);

	GLuint getHandle() const
	{
		return handle;
	}
	GLint getStatus() const
	{
		return status;
	}

private:
	GLuint handle;
	GLint status;
};

#endif // GLSHADER_H
