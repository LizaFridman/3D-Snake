
#version 130

varying vec2 texCoord0;
varying vec3 normal0;
varying vec4 color0;
varying vec3 weight0;

out vec4 FragColor;

uniform vec3 lightDirection;
uniform vec3 lightColor;

void main()
{
    //vec3 tmp = dot(-lightDirection, normal0) * color0 ;
	//vec3 texColor = texture(u_Texture, texCoord0).rgb;
	//gl_FragColor = texture2D(u_Texture, texCoord0) *clamp(vec4(color0,1.0), 0.0, 1.0);
    //gl_FragColor = clamp(vec4(color0,1.0), 0.0, 1.0);//clamp(texColor,0.0,1.0);

	//gl_FragColor = texture2D(u_Texture, texCoord0) * clamp(vec4(color0,1.0), 0.0, 1.0);;
	FragColor = color0;//clamp(vec4(color0,1.0), 0.0, 1.0);

}