#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	vec3 worldPosition;
	vec3 worldNormal;
}fs_in;

uniform int numWaves;
uniform float time;
uniform float speed;

uniform sampler2D _Texture;
uniform sampler2D _CellTexture;
uniform sampler2D _NoiseTexture;

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
	float rimK;
	float shininess;
	float rimAmbientIntesity;
};

uniform Material _Material;

uniform vec3 cameraPos;


void main(){

	
	vec3 worldNormal = normalize(fs_in.worldNormal);

	vec3 worldTolightVector;

	vec3 worldToCamera = normalize(cameraPos-fs_in.worldPosition);

	vec3 cellColor;
	
	vec2 newUV = fs_in.UV;

	float noise = texture(_NoiseTexture, fs_in.UV * numWaves + time/5 * 0.5f).r;
	
	//trying out a new specular equation
	//vec3 specular = texture(_SpecularTexture, vec2(r.x+worldNormal.x,r.y + worldNormal.y)).rgb;

	//cellColor =+ specular;
	//newUV.y = fs_in.UV.y + time * speed/100;
	//newUV.x = fs_in.UV.x + time * speed/100;


	// Lighting taken from defaultLit.frag by Will Mansfield
	for(int i = 0; i < numLights; i++)
	{
	 worldTolightVector = normalize(_Lights[i].position - fs_in.worldPosition );

	 //ambient 
	 cellColor += _Material.ambientK * _Lights[i].color;

	 //diffuse 
	 cellColor +=  _Material.diffuseK * _Lights[i].color * texture(_CellTexture, vec2(max(dot(worldTolightVector, worldNormal),0)),0).rgb;

	 //specualr
	 vec3 r;
	 r = normalize(worldTolightVector + worldToCamera);
	float specular = sin(pow(max(dot(r,worldNormal) * 1,0 ),_Material.shininess));
	vec3 SpecLocation = normalize(r + worldToCamera);
	specular = step(0.2,specular);
	cellColor += _Lights[i].color *_Material.specular * specular;

	//rim Lighting
	float rimLightIntens = dot(worldToCamera, worldNormal);
	rimLightIntens = max(0.0, rimLightIntens);
	rimLightIntens = 1.0 - rimLightIntens;
	rimLightIntens = pow(rimLightIntens,_Material.rimAmbientIntesity);
	rimLightIntens *= _Material.rimK;
	vec3 rimLight = _Lights[i].color * rimLightIntens;
	cellColor += rimLight;
	}

	newUV += noise * 0.1f;
	
	FragColor = vec4(cellColor,1.0) * texture(_Texture,newUV * numWaves);
	//FragColor = texture(_Texture,fs_in.UV);
 }