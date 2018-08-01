#version 130

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;
varying vec3 weight0;
varying vec4 position0;

out vec4 FragColor;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform mat4 Normal;
uniform vec4 eyePosition;

uniform sampler2D sampler;

void main()
{
	vec3 texColor = texture2D(sampler, texCoord0).rgb;

    vec3 L = normalize(lightDirection);
	vec3 diffuseColor = lightColor * max(dot(-L, normal0), 0);
	vec4 intersection = Normal * vec4(position0.xyz, 1.0);
	vec3 v = normalize(eyePosition - intersection).xyz;
	vec3 r = reflect(L, normalize(normal0));
	float specularColor = max(dot(v,r), 0);
	vec3 intensity = diffuseColor + pow(specularColor, 10)*vec3(0.2,0.2,0.2);

    float textureFactor = 0.3;
    float intensityFactor = 1-textureFactor;
	vec3 finalColor = intensityFactor * intensity + textureFactor * texColor;
	FragColor = clamp(vec4(finalColor, 1.0), 0.0, 1.0);

}