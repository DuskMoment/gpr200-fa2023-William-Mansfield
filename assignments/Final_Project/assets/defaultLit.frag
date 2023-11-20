#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	vec3 worldPosition;
	vec3 worldNormal;
}fs_in;

uniform sampler2D _Texture;
uniform sampler2D _CellTexture;

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

uniform vec3 cameraTarget;

uniform int _ifCameraLocked;


void main(){

	
	vec3 worldNormal = normalize(fs_in.worldNormal);

	vec3 worldTolightVector;

	vec3 cellColor;

	
	
	 worldTolightVector = normalize(_Lights[0].position - fs_in.worldPosition );

	 //ambient 
	 cellColor += _Material.ambientK * vec3(1.0,1.0,1.0);
	 //diffuse 
	 cellColor +=  _Material.diffuseK * texture(_CellTexture, vec2(max(dot(worldTolightVector, worldNormal),0)),0).rgb;

	 vec3 r;
	  r = normalize(worldTolightVector + normalize(cameraPos - fs_in.worldPosition));
	 cellColor += vec3(1.0,1.0,1.0) * _Material.specular * pow(max(dot(r,worldNormal),0),_Material.shininess);
	vec3 color;
//	if(dot(worldTolightVector,worldNormal) > 0.5)
//	{
//		color = vec3(1.0,1.0,1.0);
//	}
//	else if(dot(worldTolightVector,worldNormal) > 0.3)
//	{
//		
//		color = vec3(0.33,0.33,0.33);
//	}
//
//	else if(dot(worldTolightVector,worldNormal) >0.0)
//	{
//		color = vec3(0.1,0.1,0.1);
//	}
//	else 
//	{
//		color = vec3(0.0,0.0,0.0);
//	}
//	
	FragColor = vec4(cellColor,1.0) * texture(_Texture,fs_in.UV);
 }