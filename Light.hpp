#ifndef LIGHT_HPP_INCLUDED
#define LIGHT_HPP_INCLUDED

#include "Includes.h"

#include <glm/glm.hpp>

struct PointLight
{
    glm::vec4 intensity;
    glm::vec3 position;
    float attenuation;

    static const std::size_t firstAlignmentStd430 = 4 * sizeof(GLfloat);
    static const std::size_t sizeStd430 = 8 * sizeof(GLfloat);
    static constexpr std::size_t calcAlignedOffsetStd430(std::size_t offset)
    {
        return offset % firstAlignmentStd430 ?
            offset - offset % firstAlignmentStd430 + firstAlignmentStd430 :
            offset;
    }
    static constexpr std::size_t calcArraySizeStd430(std::size_t num)
    {
        return num <= 1 ?
            sizeStd430 :
            calcAlignedOffsetStd430(calcArraySizeStd430(num - 1)) +
            sizeStd430;
    }

    std::size_t writeStd430(void *buffer, std::size_t offset, std::size_t size);
    std::size_t writeStd430Transform(void *buffer, std::size_t offset, std::size_t size,
                                     const glm::mat4& transformation);
};

struct DirectionalLight
{
    glm::vec4 intensity;
    glm::vec3 direction;
};

#endif // LIGHT_HPP_INCLUDED
