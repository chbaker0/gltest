#include "Shader.hpp"

void Shader::loadSource(const std::string& source)
{
	const char *source_c_str = source.c_str();
	glShaderSource(handle, 1, &source_c_str, nullptr);
}
void Shader::loadSource(std::ifstream& source_file)
{
	std::string source;
	while(source_file)
		source.push_back(source_file.get());
	loadSource(source);
}
void Shader::loadSource(std::ifstream&& source_file)
{
	loadSource(source_file);
}

bool Shader::compile(std::string *log)
{
	glCompileShader(handle);

	glGetShaderiv(handle, GL_COMPILE_STATUS, &status);

	if(log)
	{
		GLint len;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
		log->resize(len+1);
		glGetShaderInfoLog(handle, len, nullptr, &(*log)[0]);
		log->pop_back();
	}

	return status;
}
