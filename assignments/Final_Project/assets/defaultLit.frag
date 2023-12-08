#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	vec3 worldPosition;
	vec3 worldNormal;
	float yPos;
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
	// Will Mansfield added rim lighting variables
	float rimK;
	float shininess;
	float rimAmbientIntesity;
};

uniform Material _Material;

uniform vec3 cameraPos;


void main(){
	// William Mansfield Cel Shader Lighting
	
	vec3 worldNormal = normalize(fs_in.worldNormal);

	vec3 worldTolightVector;

	vec3 worldToCamera = normalize(cameraPos-fs_in.worldPosition);

	vec3 cellColor;

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

	// William Mansfield rim lighting
	//rim Lighting
	float rimLightIntens = dot(worldToCamera, worldNormal);

	rimLightIntens = max(0.0, rimLightIntens);
	rimLightIntens = 1.0 - rimLightIntens;

	//rim intestity
	rimLightIntens = pow(rimLightIntens,_Material.rimAmbientIntesity);
	//rimLightIntens = smoothstep(0.3,0.4,rimLightIntens);
	rimLightIntens *= _Material.rimK;
	vec3 rimLight = _Lights[i].color * rimLightIntens;
	
	cellColor += rimLight;
	}
	
	// William Mansfield change mountain color based on height
	vec2 y = vec2(0, fs_in.yPos);

	FragColor = vec4(cellColor,1.0) * texture(_Texture,y);
	// FragColor = texture(_Texture,fs_in.UV);
 }