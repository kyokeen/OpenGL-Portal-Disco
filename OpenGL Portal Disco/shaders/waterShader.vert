#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fNormal;
out vec4 fPosEye;
out vec4 fPos;
out vec2 fTexCoords;
out vec4 fragPosLightSpace;
out vec4 fragPosLightSpace2;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform	mat3 normalMatrix;

uniform mat4 lightSpaceTrMatrix;
uniform mat4 lightSpaceTrMatrix2;

uniform float time;

float generateOffset(float x, float z)
{
	float radiansx = (x / 6 + time / 10) * 2.0 * 3.14;
	float radiansy = (z / 6 + time / 10) * 2.0 * 3.14;
	return 0.3 * 1.5 * (0.5 * sin(radiansx) + 0.3 * cos(radiansy));
} 

void main() 
{
	vec4 pos = vec4(vPosition.x, generateOffset(vPosition.x,vPosition.z), vPosition.z, 1.0f);

	gl_Position = projection * view * model * pos;

	fPosEye = view * model * pos;
	fPos = model * pos;
	fNormal = normalize(normalMatrix * vNormal);
	fTexCoords = vTexCoords;
	fragPosLightSpace = lightSpaceTrMatrix * model * pos;

}
