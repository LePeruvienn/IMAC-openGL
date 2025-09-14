#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

uniform float uTime;

mat3 rotate(float a) {

	mat3 R = mat3(vec3(cos(a), sin(a), 0), vec3(-sin(a), cos(a), 0), vec3(0, 0, 1));

	return R;
}

void main() {

	mat3 R = rotate(uTime);
	vec2 transfomed = vec3(R * vec3(aVertexPosition, 1)).xy;

	gl_Position = vec4(transfomed, 0, 1);
}
