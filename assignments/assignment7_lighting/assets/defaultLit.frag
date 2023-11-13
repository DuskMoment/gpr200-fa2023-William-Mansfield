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
uniform int numLights;

struct Material
{
	float ambientK;
	float diffuseK;
	float specular;

	float shininess;
};

uniform Material _Material;

uniform vec3 cameraPos;

uniform int ifPhong;


void main(){

//normalizing the camera vector from the vertex shader
	
	vec3 normal = normalize(fs_in.worldNormal);
	vec3 postionNormal = normalize(_Lights[0].position-fs_in.worldPosition);
	
	//color
	vec3 color;
	
	for(int i = 0; i < numLights; i++)
	{
		
		postionNormal = normalize(_Lights[i].position-fs_in.worldPosition);
		normal = normalize(fs_in.worldNormal);
		//ambient 
		color += _Material.ambientK * _Lights[i].color;
		//diffusion
		color += _Material.diffuseK * _Lights[i].color * max(dot(normal,normalize(postionNormal)),0);
		//specular -- not working
		vec3 r;
		if(ifPhong == 1)
		{
			 r = normalize(postionNormal + normalize(cameraPos - fs_in.worldPosition));
			 color += _Lights[i].color * _Material.specular * pow(max(dot(r,normal),0),_Material.shininess);
		}
		else
		{
			 r = normalize(reflect(-postionNormal, normal));
			 color += _Lights[i].color * _Material.specular * pow(max(dot(r,normalize(cameraPos-postionNormal)),0),_Material.shininess);
			
		}
		
		
	}
	
	
	FragColor = vec4(color, 1.0) * texture(_Texture,fs_in.UV);
}