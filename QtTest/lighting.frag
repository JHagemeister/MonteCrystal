#version 330 core
out vec4 color;
  
in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec4 objectColor;

void main()
{
    // Ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir),0);
    vec3 diffuse = diff * lightColor;
	
	vec3 tmp = vec3(objectColor.r, objectColor.g, objectColor.b);

    vec3 result = (ambient + diffuse) * tmp;
    color = vec4(result, objectColor.a);
} 