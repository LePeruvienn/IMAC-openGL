#version 330 core

in vec3 vFragColor;
in vec2 vFragPosition;

out vec3 fFragColor;


const vec2 center = vec2(0, 0);

void main() {

	
	// - Formule d'attenuation : attenuation = alpha * exp(-beta * distance^2)

	float d = distance(vFragPosition, center);
	
	float alpha = 2;
	float beta = 20;
	
	float a = alpha * exp(-beta * d * d);

	// - Différentes façon de générer des textures procéduralement
	// float a = length(fract(5 * vFragPosition));
	// float a = length(abs(fract(5.0 * vFragPosition) * 2.0 - 1.0));
	// float a = mod(floor(10.0 * vFragPosition.x) + floor(10.0 * vFragPosition.y), 2.0);
	// float a = smoothstep(0.3, 0.32, length(fract(5.0 * vFragPosition) - 0.5));
	// float a = smoothstep(0.4, 0.5, max(abs(fract(8.0 * vFragPosition.x - 0.5 * mod(floor(8.0 * vFragPosition.y), 2.0)) - 0.5), abs(fract(8.0 * vFragPosition.y) - 0.5)));

	fFragColor = vFragColor * a;
}
