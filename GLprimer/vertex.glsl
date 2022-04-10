#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Color;
out vec3 interpolatedColor;
uniform float time;
//uniform mat4 T;
//uniform mat4 R;
uniform mat4 Res;

void main() {
    //gl_Position = vec4(0.1, 0.4, 0.5, 1.0)*vec4(Position, 1.0) + 0.8f;
    //gl_Position = sin(time)*vec4(Position, 1.0) + 0.2f;
    //gl_Position = 0.5f*vec4(Position, 1.0) + 0.8f*sin(time);
    //gl_Position = T * R*vec4(Position, 1.0);
    gl_Position = Res*vec4(Position, 1.0);

    interpolatedColor = Color; // Pass interpolated color to fragment shader
}
