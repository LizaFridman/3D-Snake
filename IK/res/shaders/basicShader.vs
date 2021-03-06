#version 130

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 color;

attribute vec3 normal;
attribute vec3 weight;

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;
varying vec3 weight0;
varying vec4 position0;

uniform mat4 MVP;
uniform mat4 Normal;
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform mat4[20] Tjs;
uniform int boneIndex;
uniform int bonesNum;
uniform vec4 eyePosition;

mat4 calcWeights(){
	
	mat4 pre = mat4(0);
	mat4 curr = mat4(0);
	mat4 post = mat4(0);

	if(boneIndex == 0){
		curr = weight.y * Tjs[boneIndex];
		post = weight.z * Tjs[boneIndex + 1];
	}
	else if(boneIndex == bonesNum - 1){
		pre = weight.x * Tjs[boneIndex - 1];
		curr = weight.y * Tjs[boneIndex];
	}else{
		pre = weight.x * Tjs[boneIndex - 1];
		curr = weight.y * Tjs[boneIndex];
		post = weight.z * Tjs[boneIndex + 1];
	}

	return pre + curr + post;
}

void main()
{
	if( boneIndex < bonesNum){
		gl_Position = calcWeights() * vec4(position, 1.0);
	}else{
		gl_Position = MVP * vec4(position, 1.0);
	}

	texCoord0 = texCoord;
	color0 = color;
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
	weight0 = weight;
	position0 = gl_Position;
}

