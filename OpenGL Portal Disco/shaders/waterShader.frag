#version 410 core

#define NO_SPOTLIGHTS 17
#define NO_POINTLIGHTS 3

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;

in vec4 fPos;

out vec4 fColor;

uniform vec3 viewPos;

//lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;

uniform vec3 pointLightPositions[NO_POINTLIGHTS];
uniform vec3 pointLightColors[NO_POINTLIGHTS];
uniform float pointLightConsts[NO_POINTLIGHTS];
uniform float pointLightLinears[NO_POINTLIGHTS];
uniform float pointLightQuads[NO_POINTLIGHTS];

uniform vec3 spotLightPositions[NO_SPOTLIGHTS];
uniform vec3 spotLightDirections[NO_SPOTLIGHTS];
uniform vec3 spotLightColors[NO_SPOTLIGHTS];
uniform float spotLightConsts[NO_SPOTLIGHTS];
uniform float spotLightLinears[NO_SPOTLIGHTS];
uniform float spotLightQuads[NO_SPOTLIGHTS];
uniform float spotCutoffs[NO_SPOTLIGHTS];
uniform float spotOuterCutoffs[NO_SPOTLIGHTS];


//texture
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;

vec3 ambient;
float ambientStrength = 0.0005f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.05f;
float shininess = 32.0f;

void computeDirectionalLight()
{		
	//ambient
	ambient += ambientStrength * lightColor;

	//diffuse
	vec3 norm = normalize(fNormal);
	vec3 lightDir_normalized = normalize(-lightDir); //directional light
	diffuse += max(dot(norm, lightDir_normalized), 0.0f) * lightColor;

	//spec
	vec3 viewDir_normalized = normalize(-fPos.xyz);
	vec3 reflection = reflect(-lightDir_normalized, norm);
	float spec = pow(max(dot(viewDir_normalized, reflection), 0.0f), shininess);
	specular += specularStrength * spec * lightColor;

}

void computePointLight(vec3 lightPosition, vec3 lightColor, float constant, float lin, float quad) {
	float distance = length(lightPosition - fPos.xyz);
	float att = 1.0f / (constant + lin * distance + quad * distance * distance);

	//ambient
	ambient += att * ambientStrength * lightColor;

	//diffuse
	vec3 norm = normalize(fNormal);
	vec3 lightDir_normalized = normalize(lightPosition - fPos.xyz); //point light
	diffuse += att * max(dot(norm, lightDir_normalized), 0.0f) * lightColor;

	//spec
	vec3 viewDir_normalized = normalize(viewPos - fPos.xyz);
	vec3 reflection = reflect(-lightDir_normalized, norm);
	float spec = pow(max(dot(viewDir_normalized, reflection), 0.0f), shininess);
	specular += att * specularStrength * spec * lightColor;
}

void computeSpotLight(vec3 lightPosition, vec3 lightDirection, vec3 lightColor, float constant, float lin, float quad, float cutoff, float outerCutoff) {
	float distance = length(lightPosition - fPos.xyz);
	float att = 1.0f / (constant + lin * distance + quad * distance * distance);
	

	//ambient
	ambient += att * ambientStrength * lightColor;

	//diffuse
	vec3 norm = normalize(fNormal);
	vec3 lightDir_normalized = normalize(lightPosition - fPos.xyz); //point light

	float theta = dot(lightDir_normalized, normalize(-lightDirection));
	float epsilon = cutoff - outerCutoff;
	float intensity = clamp((theta - outerCutoff) / epsilon, 0.0, 1.0);

	diffuse += intensity * att * max(dot(norm, lightDir_normalized), 0.0f) * lightColor;


	//spec
	vec3 viewDir_normalized = normalize(viewPos - fPos.xyz);
	vec3 reflection = reflect(-lightDir_normalized, norm);
	float spec = pow(max(dot(viewDir_normalized, reflection), 0.0f), shininess);
	specular += intensity* att * specularStrength * spec * lightColor;
}


float computeShadow()
{
	// perform perspective divide
	vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	// Transform to [0,1] range
	normalizedCoords = normalizedCoords * 0.5 + 0.5;

	// Get closest depth value from light's perspective
	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;

	// Get depth of current fragment from light's perspective
	float currentDepth = normalizedCoords.z;

	if (normalizedCoords.z > 1.0f)
		return 0.0f;

	// Check whether current frag pos is in shadow
	float bias = max(0.05f * (1.0f - dot(fNormal, lightDir)), 0.005f);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadow;
}

void main() 
{
	ambient = vec3(0.0f, 0.0f, 0.0f);
	diffuse = vec3(0.0f, 0.0f, 0.0f);
	specular = vec3(0.0f, 0.0f, 0.0f);

	//computeDirectionalLight();
	for(int i = 0; i< NO_POINTLIGHTS; i++) {
		computePointLight(pointLightPositions[i], pointLightColors[i], pointLightConsts[i], pointLightLinears[i], pointLightQuads[i]);
	}

	for(int i = 0; i< NO_SPOTLIGHTS; i++) {
		computeSpotLight(spotLightPositions[i], spotLightDirections[i], spotLightColors[i], 
			spotLightConsts[i], spotLightLinears[i], spotLightQuads[i], spotCutoffs[i], spotOuterCutoffs[i]);
	}


	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	specular *= texture(specularTexture, fTexCoords).rgb;

	//modulate with shadow
	float shadow = computeShadow();
	vec3 color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);

	//vec3 color = ambient + diffuse + specular;
    fColor = vec4(color, 1.0f);
}
