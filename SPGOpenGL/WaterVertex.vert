#version 400

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 normalVec;

uniform mat4 mvpMatrix;
uniform mat4 normalMatrix;
uniform float time;

#define WIND_DIR 0.785 

out vec3 normal;
out vec3 pos;

vec3 calculateWave(vec2 p, float angle, float wavelength, float speed, float amp) {

    float totalAngle = angle + WIND_DIR;
    float k = 2.0 * 3.14159 / wavelength;
    vec2 dir = vec2(cos(totalAngle), sin(totalAngle));
    
    float phase = k * dot(dir, p) - (time * speed);
    float wave = exp(sin(phase) - 1.0); 
    
    float h = wave * amp;
    float df = k * h * cos(phase); 
    
    return vec3(h, dir.x * df, dir.y * df);
}

void main() {
    vec3 v = vPos;
    vec2 p = v.xz;

    vec3 w1 = calculateWave(p, 0.0, 15.0, 1.1, 6);   
    vec3 w2 = calculateWave(p, 0.5, 7.3, 1.8,2.5);   
    vec3 w3 = calculateWave(p, -0.4, 3.1, 2.5, 1);  

    v.y = w1.x + w2.x + w3.x;

    float dx = w1.y + w2.y + w3.y;
    float dz = w1.z + w2.z + w3.z;


    vec3 rawNormal = normalize(vec3(-dx, 1.5, -dz)); 
    vec3 blendedNormal = mix(vec3(0.0, 1.0, 0.0), rawNormal, 0.8);
    
    normal = normalize(mat3(normalMatrix) * blendedNormal);
    pos = v;
    gl_Position = mvpMatrix * vec4(v, 1.0);
}