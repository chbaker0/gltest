#version 430

in vec3 fragNormal;
in vec3 modelSpacePosition;
in vec2 fragTexCoord;

out vec4 outColor;

layout (location = 2) uniform vec4 color;

layout (binding = 0) uniform sampler2D diffuseTex;

void main()
{
    vec3 modelSpaceLightPosition = vec3(0.0, -1.5, 1.5);

    vec4 diffuseColor = texture(diffuseTex, fragTexCoord);
    vec3 lightDirection = normalize(modelSpaceLightPosition
                                    - modelSpacePosition);
    float cosAngleIncidence = dot(normalize(fragNormal),
                                  lightDirection);
    cosAngleIncidence = clamp(cosAngleIncidence, 0, 1);

    outColor = diffuseColor * cosAngleIncidence;
}
