#GEOMETRY SHADER
#GEOMETRY SHADER END

#VERTEX SHADER
#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

out vec2 fragment_uv;

void main() {
	gl_Position = vec4(vertex_position.x, vertex_position.y, 0.0f, 1.0f);
	fragment_uv = vertex_uv;
}
#VERTEX SHADER END

#FRAGMENT SHADER
#version 330 core
in vec2 fragment_uv;

out vec4 color;

uniform sampler2D frame_texture;

uniform float exposure;

void main() 
{
	const float gamma = 2.2;
	vec3 hdr_color = texture(frame_texture, fragment_uv).rgb;

	vec3 tone_mapped = vec3(1.0) - exp(-hdr_color * exposure);
	tone_mapped = pow(tone_mapped, vec3(1.0 / gamma));

	color = vec4(tone_mapped, 1.0);
}
#FRAGMENT SHADER END