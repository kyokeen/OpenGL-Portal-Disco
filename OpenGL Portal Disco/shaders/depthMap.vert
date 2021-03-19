#version 410 core
layout(location=0) in vec3 vPosition;

uniform mat4 model;
uniform mat4 lightSpaceTrMatrix;

out vec4 fragPosLightSpace;

void main()
{
    fragPosLightSpace = lightSpaceTrMatrix * model * vec4(vPosition, 1.0f);
    gl_Position = lightSpaceTrMatrix * model * vec4(vPosition, 1.0f);
}
