#version 330 core

layout(location = 0) in vec3 Position;
layout(location=1) in vec3 Normal;
layout(location=2) in vec2 TexCoord;

uniform float time;
uniform mat4 LR;
uniform mat4 MV; 
uniform mat4 P;
out vec3 lightDirection;
out vec3 interpolatedNormal;
out vec2 st;

void main() {

    gl_Position = P*MV*vec4(Position, 1.0);
	lightDirection = normalize(mat3(LR)*vec3(0.0,0.8,0.0));


	vec3 transformedNormal = mat3(MV)*Normal;
	interpolatedNormal = normalize(transformedNormal);// Will be interpolated across the triangle
	st = TexCoord; // Will also be interpolated across the triangle
}
