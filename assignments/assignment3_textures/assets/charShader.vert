#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
uniform float _Time;
out vec2 UV;
void main(){
	UV = vUV;
	gl_Position = vec4(vPos.x/2,vPos.y/2+sin(_Time),vPos.z/2,1.0);
}