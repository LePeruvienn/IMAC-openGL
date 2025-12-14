#version 330

uniform sampler2D uTexture;

in vec2 vVertexTexture;

out vec3 fFragColor;

void main() {

	vec4 texColor = texture(uTexture, vVertexTexture);
	fFragColor = texColor.rgb;
}
