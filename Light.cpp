#include "Light.hpp"

std::size_t PointLight::writeStd430(void *buffer, std::size_t offset, std::size_t size)
{
    std::size_t correctedOffset = calcAlignedOffsetStd430(offset);

    #ifdef DEBUG
    assert(correctedOffset % sizeof(GLfloat) == 0);
    assert(correctedOffset + sizeStd430 <= size);
    #endif // DEBUG

    if(correctedOffset + sizeStd430 > size)
        return false;


    GLfloat *head = (GLfloat*)(buffer) + correctedOffset/sizeof(GLfloat);

    for(std::size_t i = 0; i < 4; ++i)
        *head++ = intensity[i];
    for(std::size_t i = 0; i < 3; ++i)
        *head++ = position[i];
    *head++ = attenuation;

    return correctedOffset + sizeStd430;
}

std::size_t PointLight::writeStd430Transform(void *buffer, std::size_t offset, std::size_t size,
                                             const glm::mat4& transformation)
{
    PointLight temp(*this);
    temp.position = glm::vec3(transformation * glm::vec4(temp.position, 1.0f));
    temp.writeStd430(buffer, offset, size);
}
