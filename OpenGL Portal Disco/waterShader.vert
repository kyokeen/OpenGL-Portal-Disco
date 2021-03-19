#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fNormal;
out vec4 fPosEye;
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
float frequency = 3;

void main() 
{
	//compute eye space coordinates
	//fPosEye = view * model * vec4(vPosition, 1.0f);
	//fNormal = normalize(normalMatrix * vNormal);
	//fTexCoords = vTexCoords;
	//fragPosLightSpace = lightSpaceTrMatrix * model * vec4 (vPosition, 1.0f);
	//fragPosLightSpace2 = lightSpaceTrMatrix2 * model * vec4 (vPosition, 1.0f);

	//gl_Position = projection * view * model * vec4(vPosition, 1.0f);



	vec4 pos = vec4(vPosition, 1.0f);
	
	float amount = 0.3 * sin(2 * 3.1415 * frequency * time); //amplitude * sin(2pi*freq*time)
	pos.y = pos.y + amount;
	gl_Position = projection * view * model * pos;


	//compute eye space coordinates
	fPosEye = view * model * pos;
	fNormal = normalize(normalMatrix * vNormal);
	fTexCoords = vTexCoords;
	fragPosLightSpace = lightSpaceTrMatrix * model * pos;
	fragPosLightSpace2 = lightSpaceTrMatrix2 * model * pos;

}
