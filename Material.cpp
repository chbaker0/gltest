#include "Includes.h"

#include "Material.hpp"

void Material::apply(const ShaderProgram *prog)
{
    if(diffuse)
    {
        diffuse->bindTo(diffuseTexBindingPoint);
    }
    if(specular)
    {
        specular->bindTo(specularTexBindingPoint);
    }

    GLuint handle = prog->getHandle();

    glProgramUniform1fEXT(handle,
                          shininessUniformLocaiton,
                          shininess);
}

void Material::apply(const ShaderProgramManager *man)
{
    if(diffuse)
    {
        diffuse->bindTo(diffuseTexBindingPoint);
    }
    if(specular)
    {
        specular->bindTo(specularTexBindingPoint);
    }

    man->setGlobalUniform(shininessUniformLocaiton, shininess);
}
