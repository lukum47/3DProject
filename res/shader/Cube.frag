#version 330
out vec4 frag_color;
in vec2 texCoords;
in vec3 norm;
in vec3 fragPos;

struct Material
{
 sampler2D diffuse;
 sampler2D specular;
 sampler2D normal;
};
uniform Material materials;
void main() 
{

	vec3 result = texture(materials.diffuse, texCoords).rgb;

	frag_color = vec4(result, 1.0);

}
