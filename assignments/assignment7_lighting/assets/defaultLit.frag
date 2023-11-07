#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	vec3 worldPosition;
	vec3 worldNormal;
}fs_in;

uniform sampler2D _Texture;

struct Light
{
	vec3 position;
	vec3 color;

};
#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];

//all 1-0 ranges
uniform float ambientK;
uniform float diffuseK;
uniform float specular;
//not 1 - 0
uniform float shininess;

//gets the cameraVector from 
in vec3 cameraVector;

void main(){

//normalizing the camera vector from the vertex shader
	vec3 fCameraVector =  normalize(cameraVector);
	vec3 normal = normalize(fs_in.worldNormal);

	//test color
	vec3 color = vec3(0.0,0.0,0.0);
	
	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		vec3 reflection = reflect(-fs_in.worldPosition,normal);
		color = color + _Lights[i].color * diffuseK*max(dot(fs_in.worldPosition,normal),0) + _Lights[i].color * specular * pow(dot(reflection,cameraVector), shininess);

	}
	 
	FragColor = vec4(color,1.0) * texture(_Texture,fs_in.UV);
}