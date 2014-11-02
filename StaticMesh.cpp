#include "StaticMesh.h"

bool StaticMesh::loadFromFile(const std::string& infile)
{
    Assimp::Importer imp;

    // Load scene and grab first mesh only
    imp.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE,
                           aiPrimitiveType_LINE |
                           aiPrimitiveType_POINT |
                           aiPrimitiveType_POLYGON, nullptr);

    #ifdef DEBUG
    std::cout << "Calling Assimp::Importer::ReadFile...\n";
    #endif // DEBUG

    const aiScene *scene = imp.ReadFile(infile, aiProcessPreset_TargetRealtime_Quality
                                                & (!aiProcess_SplitLargeMeshes));
    #ifdef DEBUG
    std::cout << "Checking for a mesh...\n";
    #endif // DEBUG
    if(scene == nullptr)
        return false;
    else if(!scene->HasMeshes())
        return false;

    imp.ApplyPostProcessing(aiProcess_JoinIdenticalVertices);

    // Will only load one mesh
    const aiMesh *mesh = scene->mMeshes[0];

    #ifdef DEBUG
    std::cout << "Checking mesh has what we need...\n";
    #endif // DEBUG
    // Don't process special mesh types
    if(!mesh->HasPositions() ||
       !mesh->HasFaces())
        return false;

    vertexCount = mesh->mNumVertices;
    triangleCount = mesh->mNumFaces;

    // Check what attributes are in the mesh and
    // calculate total count of floats
    std::size_t componentCount = components.vertex = 3;
    if(mesh->HasNormals())
    {
        componentCount += components.normal = 3;
        #ifdef DEBUG
        std::cout << "Has normals\n";
        #endif // DEBUG
    }
    if(mesh->HasTangentsAndBitangents())
    {
        componentCount += (components.tangentBitangent = 3) * 2;
        #ifdef DEBUG
        std::cout << "Has tangents and bitangents\n";
        #endif // DEBUG
    }
    if(mesh->HasTextureCoords(0))
    {
        componentCount += components.texture = 3;
        #ifdef DEBUG
        std::cout << "Has texture coordinates\n";
        #endif // DEBUG
    }

    // Scale size for size of float
    std::size_t totalSize = componentCount * sizeof(float) * vertexCount;

    #ifdef DEBUG
    std::cout << totalSize << std::endl;
    std::cout << "Allocating and mapping vertex buffer...\n";
    #endif // DEBUG

    // Set size and map buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);
    GLfloat *mapPtr = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
//    glNamedBufferDataEXT(buffer, totalSize, nullptr, GL_STATIC_DRAW);
//    GLfloat *mapPtr = (GLfloat*) glMapNamedBufferEXT(buffer, GL_WRITE_ONLY);
    if(mapPtr == nullptr)
        return false;

    #ifdef DEBUG
    std::cout << "Worked!\n";
    #endif // DEBUG

    // Load all attributes into mapped buffer
    for(unsigned int i = 0; i < vertexCount; ++i)
    {
        GLfloat *head = mapPtr + i * componentCount;

        *head++ = mesh->mVertices[i].x;
        *head++ = mesh->mVertices[i].y;
        *head++ = mesh->mVertices[i].z;

        if(mesh->HasNormals())
        {
            *head++ = mesh->mNormals[i].x;
            *head++ = mesh->mNormals[i].y;
            *head++ = mesh->mNormals[i].z;
        }
        if(mesh->HasTextureCoords(0))
        {
            *head++ = mesh->mTextureCoords[0][i].x;
            *head++ = mesh->mTextureCoords[0][i].y;
            *head++ = mesh->mTextureCoords[0][i].z;
        }
        if(mesh->HasTangentsAndBitangents())
        {
            *head++ = mesh->mTangents[i].x;
            *head++ = mesh->mTangents[i].y;
            *head++ = mesh->mTangents[i].z;
            *head++ = mesh->mBitangents[i].x;
            *head++ = mesh->mBitangents[i].y;
            *head++ = mesh->mBitangents[i].z;
        }
    }

    if(!glUnmapBuffer(GL_ARRAY_BUFFER))
        return false;

    #ifdef DEBUG
    std::cout << "Allocating and mapping index buffer...\n";
    #endif // DEBUG

    // Same for index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleCount*3*sizeof(GLshort), nullptr, GL_STATIC_DRAW);
    GLushort *indexMapPtr = (GLushort*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
//    glNamedBufferDataEXT(indices, triangleCount * 3 * sizeof(GLushort), nullptr, GL_STATIC_DRAW);
//    GLushort *indexMapPtr = (GLushort*) glMapNamedBufferEXT(indices, GL_WRITE_ONLY);
    if(indexMapPtr == nullptr)
        return false;

    #ifdef DEBUG
    std::cout << "Worked!\n";
    #endif // DEBUG

    for(unsigned int i = 0; i < triangleCount; ++i)
    {
        #ifdef DEBUG
        assert(mesh->mFaces[i].mNumIndices == 3);
        #endif // DEBUG
        unsigned int *indexArray = mesh->mFaces[i].mIndices;
        for(unsigned int j = 0; j < 3; ++j)
        {
            #ifdef DEBUG
            assert(*indexArray <= std::numeric_limits<GLushort>::max());
            #endif // DEBUG
            *indexMapPtr++ = *indexArray++;
        }
    }

    if(!glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER))
        return false;;

    #ifdef DEBUG
    std::cout << "Loading mesh successful!\n";
    std::cout << "Setting up vertex array...\n";
    #endif // DEBUG

    glBindVertexArray(vertexArray);

    glBindVertexBuffer(0, buffer, 0, (components.vertex +
                                      components.normal +
                                      components.texture +
                                      components.tangentBitangent * 2)
                                        * sizeof(GLfloat));
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);
    glEnableVertexAttribArray(0);

    if(components.normal)
    {
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, components.vertex * sizeof(float));
        glVertexAttribBinding(1, 0);
        glEnableVertexAttribArray(1);
    }
    if(components.texture)
    {
        glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, (components.vertex +
                                                        components.normal) * sizeof(float));
        glVertexAttribBinding(2, 0);
        glEnableVertexAttribArray(2);
    }
    if(components.tangentBitangent)
    {
        glVertexAttribFormat(3, 3, GL_FLOAT, GL_FALSE, (components.vertex +
                                                        components.normal +
                                                        components.texture) * sizeof(float));
        glVertexAttribBinding(3, 0);
        glEnableVertexAttribArray(3);

        glVertexAttribFormat(4, 3, GL_FLOAT, GL_FALSE, (components.vertex +
                                                        components.normal +
                                                        components.texture +
                                                        components.tangentBitangent) * sizeof(float));
        glVertexAttribBinding(4, 0);
        glEnableVertexAttribArray(4);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);

    glBindVertexArray(0);

    #ifdef DEBUG
    std::cout << vertexCount << ' ' << triangleCount << std::endl;
    std::cout << +components.vertex << ' ';
    std::cout << +components.normal << ' ';
    std::cout << +components.texture << ' ';
    std::cout << +components.tangentBitangent << std::endl;
    #endif // DEBUG

    return true;
}
