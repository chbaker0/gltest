#include "ShaderProgramManager.hpp"

ShaderProgramManager *globalProgramManager;

//template <>
//void ShaderProgramManager::setGlobalUniform<float>(unsigned int loc, const float& value) const
//{
//    for(auto &i : programs)
//        glProgramUniform1f(i.second->getHandle(),
//                           loc, value);
//}
//template <>
//void ShaderProgramManager::setGlobalUniform<glm::vec2>(unsigned int loc, const glm::vec2& value) const
//{
//    for(auto &i : programs)
//        glProgramUniform2fv(i.second->getHandle(),
//                           loc, 1, glm::value_ptr(value));
//}
//template <>
//void ShaderProgramManager::setGlobalUniform<glm::vec3>(unsigned int loc, const glm::vec3& value) const
//{
//    for(auto &i : programs)
//        glProgramUniform3fv(i.second->getHandle(),
//                           loc, 1, glm::value_ptr(value));
//}
//template <>
//void ShaderProgramManager::setGlobalUniform<glm::vec4>(unsigned int loc, const glm::vec4& value) const
//{
//    for(auto &i : programs)
//        glProgramUniform4fv(i.second->getHandle(),
//                           loc, 1, glm::value_ptr(value));
//}
//template <>
//void ShaderProgramManager::setGlobalUniform<glm::mat2>(unsigned int loc, const glm::mat2& value) const
//{
//    for(auto &i : programs)
//        glProgramUniformMatrix2fv(i.second->getHandle(), loc,
//                                  1, GL_FALSE, glm::value_ptr(value));
//}
//template <>
//void ShaderProgramManager::setGlobalUniform<glm::mat3>(unsigned int loc, const glm::mat3& value) const
//{
//    for(auto &i : programs)
//        glProgramUniformMatrix3fv(i.second->getHandle(), loc,
//                                  1, GL_FALSE, glm::value_ptr(value));
//}
//template <>
//void ShaderProgramManager::setGlobalUniform<glm::mat4>(unsigned int loc, const glm::mat4& value) const
//{
//    for(auto &i : programs)
//        glProgramUniformMatrix4fv(i.second->getHandle(), loc,
//                                  1, GL_FALSE, glm::value_ptr(value));
//}
