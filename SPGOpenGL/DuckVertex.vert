#version 400

layout (location = 0) in vec3 aPos;       // Trebuie sa fie 0
layout (location = 1) in vec3 aNormal;    // Trebuie sa fie 1
layout (location = 2) in vec2 aTexCoords; // Trebuie sa fie 2

out vec3 normal;
out vec3 pos;
out vec2 TexCoords;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;

void main() 
{
    gl_Position = mvpMatrix * vec4(aPos, 1.0);
    
    // Trimitem pozitia si normalele catre Fragment Shader
    pos = vec3(modelMatrix * vec4(aPos, 1.0)); 
    normal = mat3(transpose(inverse(modelMatrix))) * aNormal; 
    TexCoords = aTexCoords;
}