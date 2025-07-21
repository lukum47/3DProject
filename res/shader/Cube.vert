#version 330
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 texture_coords;
out vec2 texCoords;
out vec3 norm;
out vec3 fragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() 
{
	
	texCoords = texture_coords;
	norm = Normal;
	fragPos = vec3(model * vec4(vertex_position, 1.0));
	gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}
