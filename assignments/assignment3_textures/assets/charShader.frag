#version 450
out vec4 FragColor;
in vec2 UV;

uniform float _Time;
uniform sampler2D _FaceTexture;

void main(){

	
	float time = _Time;
	vec4 color = texture(_FaceTexture,UV);

	color = mix(color,vec4(color.r,color.g,color.b,0.0),sin(time));

	FragColor = color;


}