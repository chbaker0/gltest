#version 430

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texCoord;

layout (location = 0) uniform mat4 modelCameraTransform;
layout (location = 1) uniform mat4 cameraClipTransform;

out vec3 fragNormal;
out vec3 modelSpacePosition;
out vec2 fragTexCoord;

void main()
{
    vec4 cameraPosition = modelCameraTransform * vec4(position.xyz, 1.0);
    gl_Position = cameraClipTransform * cameraPosition;

    modelSpacePosition = position.xyz;
    fragNormal = normal.xyz;
    fragTexCoord = texCoord;
}

