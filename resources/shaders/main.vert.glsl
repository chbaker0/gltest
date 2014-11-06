#version 440

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texCoord;

layout (location = 0) uniform mat4 modelCameraTransform;
layout (location = 1) uniform mat4 cameraClipTransform;

out vec3 fragNormal;
out vec3 cameraSpacePosition;
out vec2 fragTexCoord;

struct PointLight
{
    vec4 intensity;
    vec3 position;
    float attenuation;
};

layout (std430, binding = 0) buffer PointLights
{
    PointLight pointLights[];
};

void main()
{
    vec4 cameraPosition = modelCameraTransform * vec4(position.xyz, 1.0);
    gl_Position = cameraClipTransform * cameraPosition;

    cameraSpacePosition = cameraPosition.xyz;
    fragNormal = mat3(modelCameraTransform) * vec3(normal.xyz);
    fragTexCoord = texCoord;
}
