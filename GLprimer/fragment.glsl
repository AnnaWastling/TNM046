#version 330 core

out vec4 finalcolor;
in vec3 interpolatedNormal;
in vec2 st;
in vec3 lightDirection;

uniform float time;

void main() {
   // vec3 lightDirection = vec3(1.0,1.0,1.0);
    float shading = dot(interpolatedNormal, lightDirection);
    shading = max(0.0, shading);
    finalcolor = vec4(vec3(shading), 1.0);

}
