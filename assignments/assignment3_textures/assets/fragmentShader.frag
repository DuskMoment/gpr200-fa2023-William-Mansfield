#version 450
out vec4 FragColor;
in vec2 UV;


uniform sampler2D _NoiseTexture;
uniform sampler2D _BrickTexture;
uniform sampler2D _SmileTexture;
uniform float _Time;

void main(){

	
	float time = _Time;
	vec2 uv = UV;

	float noise = texture(_NoiseTexture,uv+time*0.5f).r;
	uv = UV + noise * 0.1f;

	vec4 brickColor = texture(_BrickTexture,uv);
	vec4 smileColor = texture(_SmileTexture,uv);
	//this is what puts the textures together
	vec3 color = mix(brickColor.rgb,smileColor.rgb,smileColor.a);

	FragColor = vec4(color,1.0);


}