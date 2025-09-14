#version 330

layout(location = 3) in vec2 aVertexPosition;

out vec2 vFragPosition;

void main() {

	vFragPosition = aVertexPosition;
	gl_Position = vec4(aVertexPosition, 0, 1);
}
