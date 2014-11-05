#ifndef MATERIAL_HPP_INCLUDED
#define MATERIAL_HPP_INCLUDED

#include "Texture.hpp"
#include "ShaderProgram.hpp"
#include "ShaderProgramManager.hpp"

class Material
{
public:
    Material(Texture2D *_diffuse, Texture2D *_specular, float _shininess):
        diffuse(_diffuse), specular(_specular), shininess(_shininess) {}
    Material(): Material(nullptr, nullptr, -1.0f) {}
    static const unsigned int diffuseTexBindingPoint = 0,
                              specularTexBindingPoint = 1;
    static const unsigned int shininessUniformLocaiton = 5;

    void setDiffuseTexture(Texture2D *_diffuse)
    {
        diffuse = _diffuse;
    }
    void setSpecularTexture(Texture2D *_specular)
    {
        specular = _specular;
    }
    void setShininess(float _shininess)
    {
        shininess = _shininess;
    }
    Texture2D* getDiffuseTexture() const
    {
        return diffuse;
    }
    Texture2D* getSpecularTexture() const
    {
        return specular;
    }
    float getShininess() const
    {
        return shininess;
    }

    void apply(const ShaderProgram *prog);
    void apply(const ShaderProgramManager *man);

private:
    Texture2D *diffuse, *specular;
    float shininess;
};

#endif // MATERIAL_HPP_INCLUDED
