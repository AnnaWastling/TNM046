#version 330 core

layout(location = 0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec2 TexCoord;

uniform float time;
uniform mat4 Res;
uniform mat4 lightRotation;
out vec3 lightDirection;
out vec3 interpolatedNormal;
out vec2 st;

void main() {

    gl_Position = vec4(Position, 1.0);
	lightDirection = vec3(1.0,1.0,1.0);

	vec3 transformedNormal = mat3(Res)*Normal;
	interpolatedNormal = normalize(transformedNormal);// Will be interpolated across the triangle
	st = TexCoord; // Will also be interpolated across the triangle
}
