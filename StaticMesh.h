#ifndef STATIC_MESH_H_INCLUDED
#define STATIC_MESH_H_INCLUDED

#include "Includes.h"

class StaticMesh
{
public:
    StaticMesh(): components{0, 0, 0, 0}
    {
        glGenBuffers(1, &buffer);
        glGenBuffers(1, &indices);
        glGenVertexArrays(1, &vertexArray);
    }
    StaticMesh(const std::string& filename): StaticMesh()
    {
        loadFromFile(filename);
    }
    ~StaticMesh()
    {
        glDeleteBuffers(1, &indices);
        glDeleteBuffers(1, &buffer);
    }

    bool loadFromFile(const std::string& filename);

    GLuint getVertexBufferHandle() const
    {
        return buffer;
    }
    GLuint getIndexBufferHandle() const
    {
        return indices;
    }
    GLuint getVertexArrayHandle() const
    {
        return vertexArray;
    }
    unsigned int getVertexCount() const
    {
        return vertexCount;
    }
    unsigned int getTriangleCount() const
    {
        return triangleCount;
    }
    unsigned int getIndexCount() const
    {
        return triangleCount * 3;
    }

    void bindVertexArray() const
    {
        glBindVertexArray(vertexArray);
    }

private:
    /*
        Buffer storage always follows this order:

        1. vertices
        2. normals
        3. texture coords
        4. tangents and bitangents

        If one is missing, the same order
        is still observed

        All are stored with 3 components
        if they exist
    */
    GLuint buffer;
    GLuint indices;
    GLuint vertexArray;
    unsigned int vertexCount;
    unsigned int triangleCount;
    struct
    {
        unsigned char vertex : 3;
        unsigned char normal : 3;
        unsigned char texture : 3;
        unsigned char tangentBitangent : 3;
    } components;
};

#endif // STATIC_MESH_H_INCLUDED
