#GEOMETRY SHADER
#GEOMETRY SHADER END

#VERTEX SHADER
#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;
layout(location = 3) in vec3 vertex_tangent;

out vec3 fragment_position;
out vec2 fragment_uv;
out vec3 fragment_normal;
out vec3 fragment_tangent;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
	gl_Position = projection * view * model * vec4(vertex_position, 1);
	fragment_position = (model * vec4(vertex_position, 1.0f)).xyz;
	fragment_uv = vertex_uv;
	//fragment_normal = mat3(transpose(inverse(model))) * vertex_normal;
	fragment_normal = normalize(vec3(model * vec4(vertex_normal, 0.0)));
	fragment_tangent = normalize(vec3(model * vec4(vertex_tangent, 0.0)));
	fragment_tangent = normalize(fragment_tangent - dot(fragment_tangent, fragment_normal) * fragment_normal);
	//vec3 t = normalize(vec3(model * vec4(vertex_tangent, 0.0)));
	//vec3 n = normalize(vec3(model * vec4(vertex_normal, 0.0)));

	/*t = normalize(t - dot(t, n) * n);
	vec3 b = cross(n, t);

	TBN = mat3(t, b, n);*/
}
#VERTEX SHADER END

#FRAGMENT SHADER
#version 330 core
layout (location = 0) out vec3 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec4 g_albedo_specular;
layout (location = 3) out vec3 g_normal_map;
layout (location = 4) out vec3 g_tangent;

in vec3 fragment_position;
in vec2 fragment_uv;
in vec3 fragment_normal;
in vec3 fragment_tangent;

uniform sampler2D albedo;
uniform sampler2D specular;
uniform sampler2D normal_map;

void main() 
{
	g_position = fragment_position;
	//g_normal = normalize(fragment_normal);

	g_normal = fragment_normal;
	//g_normal = normalize(g_normal * 2.0 - 1.0);
	//g_normal = normalize(TBN * g_normal);
	g_normal = normalize(g_normal) * 0.5 + 0.5;
	g_albedo_specular.rgb = texture(albedo, fragment_uv).rgb;
	g_albedo_specular.a = texture(specular, fragment_uv).r;
	g_normal_map = texture(normal_map, fragment_uv).rgb;
	g_tangent = fragment_tangent * 0.5 + 0.5;
}
#FRAGMENT SHADER END