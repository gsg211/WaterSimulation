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
	vec3 specular_color  = pow(max(dot(R, V), 0.0), specPower) * specular;

	vec3 final_color=ambient + diffuse_color + specular_color;
	return final_color;
}

void main() 
{
	vec3 ambient = vec3(0.0,0.5,0.5);
	vec3 diffuse = vec3(0.0, 0.5, 0.5);
	vec3 specular = vec3(0.8);
	float specPower = 64;
	
	vec3 color = lighting(pos, normal, lightPos, viewPos, 
				ambient, diffuse, specular, specPower);
				
	fragColor = vec4(color, 1.0);
}
