#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

out Surface{
	vec2 UV;
	vec3 worldPosition;
	vec3 worldNormal;
	float yPos;
}vs_out;

uniform mat4 _Model;
uniform mat4 _ViewProjection;

out vec3 cameraVector;

void main(){
	vs_out.UV = vUV;

	vs_out.yPos = (vPos.y/10);
	
	vs_out.worldPosition = vec3( _Model * vec4(vPos,1.0));
	cameraVector = vPos* mat3(_ViewProjection);
	vs_out.worldNormal = transpose(inverse(mat3(_Model)))*vNormal;

	gl_Position = _ViewProjection * _Model * vec4(vPos,1.0);
}