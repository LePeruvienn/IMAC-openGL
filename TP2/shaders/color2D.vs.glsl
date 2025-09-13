#version 330 core

const float PI = 3.14159265359;

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;
out vec2 vFragPosition;

mat3 translate(float x, float y) {

	mat3 T = mat3(vec3(1, 0, 0), vec3(0, 1, 0), vec3(x, y, 1));

	return T;
};

mat3 scale(float x, float y) {

	mat3 S = mat3(vec3(x, 0, 0), vec3(0, y, 0), vec3(0, 0, 1));

	return S;
}

mat3 rotate(float a) {

	mat3 R = mat3(vec3(cos(a), sin(a), 0), vec3(-sin(a), cos(a), 0), vec3(0, 0, 1));

	return R;
}

void main() {

	vFragColor = aVertexColor;

	// - On fait le x2 pour agrandir le triangle avec une multiplication scalaire
	// - On décale un peu le triangle de 0.5 et 0.5
	// gl_Position = vec4(2 * aVertexPosition + vec2(0.5, 0.5), 0, 1);

	// - On Multiplie la taille X (largeur) par 2 et on divise la taille Y (hauteeur) par 2
	// vec2 newPos = vec2(2 * aVertexPosition.x, 0.5 * aVertexPosition.y);
	// gl_Position = vec4(newPos, 0, 1);

	// mat3 T = translate(0.5, 0.5);
	// mat3 S = scale(0.5, 0.5);
	// mat3 R = rotate(PI / 2);

	// mat3 M = T * S * R;

	// vec2 transfomed = vec3(M * vec3(aVertexPosition, 1)).xy;
	// gl_Position = vec4(transfomed, 0, 1);

	vFragPosition = aVertexPosition;

	gl_Position = vec4(aVertexPosition, 0, 1);
};

