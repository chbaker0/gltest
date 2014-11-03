#ifndef MATERIAL_HPP_INCLUDED
#define MATERIAL_HPP_INCLUDED

#include "Includes.h"

#include "Texture.hpp"
#include "ShaderProgram.hpp"

class Material
{
public:

    static const unsigned int diffuseTexBindingPoint = 0,
                              specularTexBindingPoint = 1;

private:
    Texture2D diffuse, specular;
    float shininess;
    bool _hasDiffuse, _hasSpecular;
};

#endif // MATERIAL_HPP_INCLUDED
