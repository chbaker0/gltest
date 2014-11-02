#ifndef SHADER_PROGRAM_MANAGER_HPP_INCLUDED
#define SHADER_PROGRAM_MANAGER_HPP_INCLUDED

#include "Includes.h"
#include "ShaderProgram.hpp"

class ShaderProgramManager
{
public:
    ShaderProgramManager() {}

    using ContainerType =
        std::map<std::string, std::unique_ptr<ShaderProgram>>;

    void registerProgram(std::string name, std::unique_ptr<ShaderProgram> prog)
    {
        programs.emplace(std::move(name), std::move(prog));
    }
    ShaderProgram* createProgram(std::string name)
    {
        auto newProgram = new ShaderProgram;
        programs.emplace(std::move(name),
                         std::unique_ptr<ShaderProgram>(newProgram));
        return newProgram;
    }
    void unregisterProgram(const std::string& name)
    {
        auto it = programs.find(name);
        if(it != programs.end())
            programs.erase(it);
    }

    template <typename T>
    void setGlobalUniform(unsigned int loc, const T& value);

private:
    ContainerType programs;
};

extern ShaderProgramManager *globalProgramManager;

#endif // SHADER_PROGRAM_MANAGER_HPP_INCLUDED
