#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

// uniform float uTime;
uniform mat3 uModelMatrix;
uniform vec3 uColor;

out vec3 vColor;

// mat3 rotate(float a) {

	// mat3 R = mat3(vec3(cos(a), sin(a), 0), vec3(-sin(a), cos(a), 0), vec3(0, 0, 1));

	// return R;
// }

void main() {

	vColor = uColor;

	vec2 transfomed = vec3(uModelMatrix * vec3(aVertexPosition, 1)).xy;
	gl_Position = vec4(transfomed, 0, 1);
}
