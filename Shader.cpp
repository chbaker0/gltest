#include "Shader.hpp"

void Shader::loadSource(const std::string& source)
{
	const char *source_c_str = source.c_str();
	GLint len = source.length();
	glShaderSource(handle, 1, &source_c_str, &len);
}
void Shader::loadSource(std::ifstream& source_file)
{
	std::string source;
	#ifdef DEBUG
	unsigned int cnt = 0;
	#endif // DEBUG
	char c;
	while(source_file.get(c))
    {
		source.push_back(c);
        #ifdef DEBUG
        ++cnt;
        #endif // DEBUG
    }
    #ifdef DEBUG
    if(!cnt)
        std::cerr << "No characters loaded from shader file!" << std::endl;
    #endif // DEBUG
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
