#version 450
out vec4 FragColor;
in vec2 UV;

uniform float _Time;
uniform sampler2D _FaceTexture;

void main(){

	
	float time = _Time;
	

	FragColor = texture(_FaceTexture,UV);


}