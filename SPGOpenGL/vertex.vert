#version 400
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 normalVec;

uniform mat4 mvpMatrix;
uniform mat4 normalMatrix;
uniform float time;

out vec3 normal;
out vec3 pos;
out float visibility;

const float density = 0.035;
const float gradient = 1.5;

void main() {
    vec3 v = vPos;

    v.y = sin(v.x * 0.4 + time) * 0.3 + cos(v.z * 0.5 + time * 1.2) + sin(v.x * 0.4 + time) ;
    

    vec4 worldPos = vec4(v, 1.0);
    vec4 posRelativeToCam = mvpMatrix * worldPos;
    gl_Position = posRelativeToCam;
    
    float dist = length(posRelativeToCam.xyz) * 0.1;
    visibility = exp(-pow((dist * density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);

    normal = mat3(normalMatrix) * normalVec;
    pos = v;
}