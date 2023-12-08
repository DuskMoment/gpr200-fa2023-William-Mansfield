#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

out Surface{
	vec2 UV;
	vec3 worldPosition;
	vec3 worldNormal;
}vs_out;



uniform mat4 _Model;
uniform mat4 _ViewProjection;

uniform float amplitude, wavelength, speed, time;
uniform vec2 direction;


void main()
{
	vs_out.UV = vUV;
	vec3 newPos = vPos;
	newPos.y += cos(vUV.x * wavelength + speed * time) * amplitude + sin(vUV.y * wavelength + speed * time) * amplitude;

	vs_out.worldPosition = vec3( _Model * vec4(vPos,1.0));
	vs_out.worldNormal = transpose(inverse(mat3(_Model)))*vNormal;

	gl_Position = _ViewProjection * _Model * vec4(newPos, 1.0);
}