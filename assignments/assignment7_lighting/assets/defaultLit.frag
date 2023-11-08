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
#define MAX_LIGHTS 1
uniform Light _Lights[MAX_LIGHTS];

struct Material
{
	float ambientK;
	float diffuseK;
	float specular;

	float shininess;
};

uniform Material _Material;

uniform vec3 cameraPos;


void main(){

//normalizing the camera vector from the vertex shader
	
	vec3 normal = normalize(fs_in.worldNormal);
	vec3 postionNormal = normalize(_Lights[0].position-fs_in.worldPosition);
	
	//test color
	vec3 color = _Lights[0].color;
	
//	for(int i = 0; i < MAX_LIGHTS; i++)
//	{
//		
//		color =+ _Lights[i].color * diffuseK*max(dot(fs_in.worldPosition,normal),0); //+ _Lights[i].color * specular * pow(dot(reflection,cameraVector), shininess);
//
//	}
	//ambient 
	 color = _Material.ambientK * _Lights[0].color;
	 //diffusion
	 color += _Material.diffuseK * _Lights[0].color * max(dot(normal,normalize(postionNormal)),0);
	 //specular -- not working
	 vec3 r = reflect(-postionNormal, normal);
	 color += _Lights[0].color * _Material.specular * pow(max(dot(r,normalize(cameraPos-postionNormal)),0),_Material.shininess); 
	
	FragColor = vec4(color, 1.0) * texture(_Texture,fs_in.UV);
}