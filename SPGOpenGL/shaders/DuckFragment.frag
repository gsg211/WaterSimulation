#version 400
out vec4 fragColor;

in vec3 normal;
in vec3 pos;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D textureSampler;

vec3 lighting(vec3 objectColor, vec3 pos, vec3 normal, vec3 lightPos, vec3 viewPos,
				vec3 ambient, vec3 lightColor, vec3 specular, float specPower)
{
	vec3 N = normalize(normal);                 
	vec3 L = normalize(lightPos - pos);       
	vec3 V = normalize(viewPos - pos);          
	vec3 R = reflect(-L, N);                  

	float diffCoef = max(dot(N, L), 0.0);

	float specCoef = 0.0;
	if (diffCoef > 0.0) {
		specCoef = pow(max(dot(R, V), 0.0), specPower);
	}

	vec3 ambientColor  = ambient * lightColor;
	vec3 diffuseColor  = diffCoef * lightColor;
	vec3 specularColor = specCoef * specular * lightColor;
	
	vec3 col = (ambientColor + diffuseColor + specularColor) * objectColor; 

	return clamp(col, 0.0, 1.0);
}

void main() 
{
	// Definim proprietatile materialului
	vec3 objectColor = texture(textureSampler, TexCoords).rgb; 
	vec3 lightColor  = vec3(1.0, 1.0, 1.0); // Lumina alba
	vec3 ambient     = vec3(0.2);           // Lumina ambientala
	vec3 specular    = vec3(1.0);           // Intensitatea reflexiei
	float specPower  = 256.0;                // Cat de "lucios" este obiectul
	
	// Apelam functia de iluminare
	vec3 color = lighting(objectColor, pos, normal, lightPos, viewPos, 
						  ambient, lightColor, specular, specPower);
		
	
	fragColor = vec4(color, 1.0);
}