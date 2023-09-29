#version 450
out vec4 FragColor;
in vec2 UV;


uniform sampler2D _NoiseTexture;
uniform sampler2D _BrickTexture;
uniform sampler2D _SmileTexture;
uniform float _Time;

void main(){

	
	float time = _Time;
	
	float noise = texture(_NoiseTexture,UV).r;
	vec2 uv = UV + noise * (sin(time) * 0.1f);
	vec4 colorA = texture(_BrickTexture,uv);
	vec4 colorB = texture(_SmileTexture,uv);
	//this is what puts the textures together
	vec3 color = mix(colorA.rgb,colorB.rgb,colorB.a);

	FragColor = vec4(color,1.0);


}