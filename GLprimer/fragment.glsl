#version 330 core

out vec4 finalcolor;
in vec3 interpolatedNormal;
in vec2 st;
in vec3 lightDirection;
uniform sampler2D tex; // A uniform variable to identify the texture
uniform mat4 LR;
uniform float time;

void main() {

    vec3 V = vec3(0.0,0.0,1.0);
    float n = 25.0;
    vec3 ka = vec3(0.4, 0.4, 0.4);
    vec3 Ia = vec3(1.0, 1.0, 1.0);
    vec3 kd = vec3(texture(tex,st)); 
    //vec3 kd = vec3(0.2, 0.4, 0.3);
    vec3 Id = vec3(1.0, 1.0, 1.0);
    vec3 ks = vec3(1.0, 1.0, 1.0);
    vec3 Is = vec3(1.0, 1.0, 1.0);
    
    vec3 R = 2.0*dot(interpolatedNormal,lightDirection)*(interpolatedNormal - lightDirection); // Could also have used the function reflect ()
    float dotNL = max(dot(interpolatedNormal, lightDirection),0.0); // If negative , set to zero
    float dotRV = max(dot(R, V),0.0);
    if(dotNL == 0.0)dotRV = 0.0; // Do not show highlight on the dark side
    vec3 shadedcolor = (Ia * ka) + (Id * kd * dotNL) + (Is * ks * pow(dotRV,n));

    finalcolor = vec4(shadedcolor, 1.0);
    // finalcolor = texture(tex, st); // Use the texture to set the surface color

}


