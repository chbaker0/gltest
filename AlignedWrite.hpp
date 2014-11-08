#ifndef ALIGNED_WRITE_HPP_INCLUDED
#define ALIGNED_WRITE_HPP_INCLUDED

#include "Includes.h"

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <glm/glm.hpp>

template <typename T>
struct TypeAlignmentMap;

#define DEF_ENTRY(T, x) template <> struct TypeAlignmentMap<T>{enum{value = (x)};};
DEF_ENTRY(GLfloat, 4)
DEF_ENTRY(glm::vec2, 8)
DEF_ENTRY(glm::vec3, 16)
DEF_ENTRY(glm::vec4, 16)
#undef DEF_ENTRY

template <typename T>
unsigned int constexpr GLAlignOf()
{
    return TypeAlignmentMap<T>::value;
}

#define ENABLE_IF_IS_CONTAINER(T) \
typename \
std::enable_if<\
    std::is_same<\
        typename boost::fusion::traits::is_sequence<T>::type,\
        boost::mpl::false_\
    >::value\
>::type

template <typename T, typename Dummy = void>
struct GLStd430Writer;

template <typename T>
struct GLStd430Writer<T, typename std::enable_if<
                            std::is_same<
                                typename boost::fusion::traits::is_sequence<T>::type,
                                boost::mpl::true_
                            >::value
                        >::type
                     >
{
    bool operator()(const T& data, void *buffer, std::size_t offset, std::size_t maxSize)
    {

    }
};

template <typename T>
struct GLStd430Writer<T, typename std::enable_if<
                            std::is_same<
                                typename boost::fusion::traits::is_sequence<T>::type,
                                boost::mpl::false_
                            >::value
                        >::type
                     >
{

};

#undef ENABLE_IF_IS_CONTAINER

#endif // ALIGNED_WRITE_HPP_INCLUDED