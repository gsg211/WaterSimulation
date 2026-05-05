#version 400
out vec4 fragColor;

in vec3 normal;
in vec3 pos;

uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 lighting(vec3 pos, vec3 normal, vec3 lightPos, vec3 viewPos,
				vec3 ambient, vec3 diffuse, vec3 specular, float specPower)
{
	//DIFUZA
	vec3 N = normalize(normal);
	vec3 L = normalize(lightPos - pos);
	vec3 diffuse_color = diffuse * max(dot(L,N),0);
	
	//SPECULARA
	vec3 V=normalize(viewPos-pos);
	vec3 R=reflect(-L,N);
	
	float spec = pow(max(dot(R, V), 0.0), specPower);
    vec3 specular_color = specular * spec;

    // Fresnel: more specular at grazing angles, softer head-on
    float fresnel = pow(1.0 - max(dot(N, V), 0.0), 3.0);
    specular_color *= (0.2 + 0.8 * fresnel); //20% speculara mereu + 80% din fresnel

	vec3 final_color=ambient + diffuse_color + specular_color;
	return final_color;
}

void main() 
{
	vec3 ambient = vec3(0.0, 0.05, 0.2);
	vec3 diffuse = vec3(0.0, 0.6, 0.6);
	vec3 specular = vec3(1.0);
	float specPower = 256;
	
	vec3 color = lighting(pos, normal, lightPos, viewPos, 
				ambient, diffuse, specular, specPower);
				
	fragColor = vec4(color, 1.0);
}
