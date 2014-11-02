#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include "Includes.h"

class Texture
{
public:
    Texture(GLenum _target): target(_target)
    {
        glGenTextures(1, &handle);
    }
    Texture(): Texture(GL_TEXTURE_2D) {}
    Texture(GLenum _target, const std::string& infile):
        Texture(_target)
    {
        loadFromFile(infile);
    }

    bool loadFromFile(const std::string& infile);

    GLuint getHandle() const
    {
        return handle;
    }
    void bind() const
    {
        glBindTexture(target, handle);
    }
    void bindTo(unsigned int unit) const
    {
        glBindMultiTextureEXT(GL_TEXTURE0 + unit,
                              target,
                              handle);
    }
    void setParameter(GLenum pname, int param)
    {
        glTextureParameteriEXT(handle, target, pname, param);
    }
    void setParameter(GLenum pname, int *param)
    {
        glTextureParameterivEXT(handle, target, pname, param);
    }
    void setParameter(GLenum pname, float param)
    {
        glTextureParameterfEXT(handle, target, pname, param);
    }
    void setParameter(GLenum pname, float *param)
    {
        glTextureParameterfvEXT(handle, target, pname, param);
    }

private:
    GLuint handle;
    GLenum target;
};

#endif // TEXTURE_HPP_INCLUDED
