#ifndef ALIGNED_WRITE_HPP_INCLUDED
#define ALIGNED_WRITE_HPP_INCLUDED

#include "Includes.h"

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <glm/glm.hpp>

template <typename T>
struct TypeAlignmentMap;

#define DEF_ENTRY(T, x) template <> struct TypeAlignmentMap<T>{enum{value = (x)};};
DEF_ENTRY(float, 4)
DEF_ENTRY(glm::vec2, 8)
DEF_ENTRY(glm::vec3, 16)
DEF_ENTRY(glm::vec4, 16)
#undef DEF_ENTRY

template <typename T>
unsigned int constexpr GLAlignOf()
{
    return TypeAlignmentMap<T>::value;
}

template <typename T, typename Dummy = void>
struct GLStd430Writer;

template <typename T>
struct GLStd430WriterVisitor;

template <typename T>
struct GLStd430Writer<T, typename std::enable_if<
                            std::is_same<
                                typename boost::fusion::traits::is_sequence<T>::type,
                                boost::mpl::true_
                            >::value
                         >::type
                     >
{
    char *buffer;
    std::size_t bufferSize;
    std::size_t offset;

    GLStd430Writer(char *_buffer, std::size_t _bufferSize, std::size_t _offset):
        buffer(_buffer), bufferSize(_bufferSize), offset(_offset) {}
    char* operator()(const T& data)
    {

        boost::fusion::for_each(data, GLStd430WriterVisitor<T>(*this));
        return buffer;
    }
};

template <typename T>
struct GLStd430WriterVisitor
{
    GLStd430Writer<T>& parent;

    GLStd430WriterVisitor(GLStd430Writer<T>& _parent): parent(_parent) {}
    template <typename U>
    void operator()(const U& e) const
    {
        parent.buffer =
            GLStd430Writer<U>(parent.buffer,
                              parent.bufferSize,
                              parent.offset)(e);
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
    char *buffer;
    std::size_t bufferSize;
    std::size_t offset;

    GLStd430Writer(char *_buffer, std::size_t _bufferSize, std::size_t _offset):
        buffer(_buffer), bufferSize(_bufferSize), offset(_offset) {}
    char* operator()(const T& data)
    {
        std::size_t alignment = GLAlignOf<T>();
        std::size_t fixedOffset = offset % alignment ? offset - offset % alignment : offset;
        if(fixedOffset + sizeof(T) >= bufferSize)
            return buffer;
        std::memcpy(buffer + fixedOffset, &data, sizeof(T));
        return buffer + fixedOffset + sizeof(T);
    }
};

#endif // ALIGNED_WRITE_HPP_INCLUDED
