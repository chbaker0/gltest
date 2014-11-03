#version 440

in vec3 fragNormal;
in vec3 cameraSpacePosition;
in vec2 fragTexCoord;

out vec4 outColor;

layout (location = 2) uniform vec4 color;

layout (binding = 0) uniform sampler2D diffuseTex;

layout (std430, binding = 0) buffer PointLights
{
    struct PointLight
    {
        vec4 intensity;
        vec3 position;
        float attenuation;
    } pointLights[];
};

void main()
{
    vec4 diffuseColor = texture(diffuseTex, fragTexCoord);
    vec4 specularColor = vec4(1.0);

    vec3 surfaceNormal = normalize(fragNormal);

    vec4 totalDiffuseLighting = vec4(0, 0, 0, 0);
    vec4 totalSpecularLighting = vec4(0, 0, 0, 0);
    for(int i = 0; i < pointLights.length(); ++i)
    {
        vec3 fragToLight = pointLights[i].position - cameraSpacePosition;
        vec3 lightDirection = normalize(fragToLight);
        float lightDistance = length(fragToLight);
        float cosAngleIncidence = dot(surfaceNormal,
                                      lightDirection);
        cosAngleIncidence = clamp(cosAngleIncidence, 0, 1);

        vec3 viewDirection = normalize(-vec3(cameraSpacePosition));
        vec3 halfAngleVector = normalize(viewDirection + lightDirection);
        float halfAngle = acos(dot(halfAngleVector, surfaceNormal));
        float exponent = halfAngle / 0.2;
        float gaussian = exp(-(exponent * exponent));
        gaussian = cosAngleIncidence != 0.0f ? gaussian : 0.0f;

        totalDiffuseLighting +=
            (diffuseColor * cosAngleIncidence * pointLights[i].intensity) / (lightDistance * lightDistance);

        totalSpecularLighting += gaussian * pointLights[i].intensity / (lightDistance * lightDistance);
    }

    outColor = totalDiffuseLighting + totalSpecularLighting;
}
