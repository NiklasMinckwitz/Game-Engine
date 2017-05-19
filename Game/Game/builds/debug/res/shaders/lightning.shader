#GEOMETRY SHADER
#GEOMETRY SHADER END

#VERTEX SHADER
#version 330 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

out vec2 fragment_uv;

void main()
{
	gl_Position = vec4(vertex_position, 1.0f);
	fragment_uv = vertex_uv;
}
#VERTEX SHADER END

#FRAGMENT SHADER
#version 330 core
out vec4 color;
in vec2 fragment_uv;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo_specular;
uniform sampler2D g_normal_map;
uniform sampler2D g_tangents;

struct PointLight {
	vec3 position;
	vec3 color;

	float intensity;

	float constant;
	float linear;
	float quadratic;
};

struct DirectionalLight {
	vec3 direction;
	vec3 color;

	float intensity;

	bool cast_shadows;
	mat4 light_space;
	sampler2D shadow_map;
};

const int NR_POINTLIGHTS = 32;
const int NR_DIRECTIONALLIGHTS = 32;

uniform int current_pointlights;
uniform int current_directionallights;

uniform PointLight pointlights[NR_POINTLIGHTS];
uniform DirectionalLight directionallights[NR_DIRECTIONALLIGHTS];

uniform vec3 view_position;

float directional_light_shadow_calculation(int l, vec4 pos, float bias);

void main()
{
	const float gamma = 2.2;

	vec3 fragment_position = texture(g_position, fragment_uv).rgb;
	vec3 normal = texture(g_normal, fragment_uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	vec3 fragment_normal = texture(g_normal_map, fragment_uv).rgb;
	fragment_normal = normalize(fragment_normal * 2.0 - 1.0);
	vec3 fragment_tangent = texture(g_tangents, fragment_uv).rgb;
	fragment_tangent = normalize(fragment_tangent * 2.0 - 1.0);
	vec3 fragment_albedo = pow(texture(g_albedo_specular, fragment_uv).rgb, vec3(gamma));
	float fragment_specular = texture(g_albedo_specular, fragment_uv).a;

	vec3 view_direction = normalize(view_position - fragment_position);

	vec3 lightning = fragment_albedo * vec3(0.01, 0.01, 0.01);

	mat3 TBN = mat3(fragment_tangent, cross(normal, fragment_tangent), normal);
	fragment_normal = normalize(TBN * fragment_normal);

	for (int i = 0; i < current_pointlights; ++i) {
		vec3 light_dir = normalize(pointlights[i].position - fragment_position);

		vec3 ambient = vec3(0.0, 0.0, 0.0); // fragment_albedo * vec3(0.0, 0.0, 0.0);

		vec3 diffuse = fragment_albedo * max(dot(fragment_normal, light_dir), 0.0) * pointlights[i].color;

		vec3 halfway_dir = normalize(light_dir + view_direction);
		float spec = pow(max(dot(fragment_normal, halfway_dir), 0.0), 16.0);
		vec3 specular = pointlights[i].color * spec * fragment_specular;

		float distance = length(pointlights[i].position - fragment_position);
		float attenuation = pointlights[i].intensity / (pointlights[i].constant + pointlights[i].linear * distance + pointlights[i].quadratic * (distance * distance));

		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		lightning += 1.0 * (ambient + diffuse + specular);
	}

	for (int i = 0; i < current_directionallights; ++i) {
		vec3 light_dir = normalize(-directionallights[i].direction);
		if (dot(fragment_normal, light_dir) > 0.0) {
			vec3 ambient = fragment_albedo * vec3(0.001, 0.001, 0.001);

			vec3 diffuse = fragment_albedo * max(dot(fragment_normal, light_dir), 0.0) * directionallights[i].color;

			vec3 halfway_dir = normalize(light_dir + view_direction);
			float spec = pow(max(dot(fragment_normal, halfway_dir), 0.0), 16.0);
			vec3 specular = directionallights[i].color * spec * fragment_specular;
		
			ambient *= directionallights[i].intensity;
			diffuse *= directionallights[i].intensity;
			specular *= directionallights[i].intensity;

			float shadow = 1.0;
			float bias = max(0.05 * (1.0 - dot(normal, light_dir)), 0.005);
			bias = 0;
			if (directionallights[i].cast_shadows) {
				shadow = directional_light_shadow_calculation(i, directionallights[i].light_space * vec4(fragment_position, 1.0), bias);
			}

			lightning += shadow  * (ambient + diffuse + specular);
		}
	}

	lightning = pow(lightning, vec3(1.0 / gamma));
	
	color = vec4(lightning, 1.0);
}

float directional_light_shadow_calculation(int l, vec4 pos, float bias) {
	vec3 proj_coords = pos.xyz / pos.w;
	proj_coords = proj_coords * 0.5 + 0.5;

	float shadow = 0.0;
	vec2 texel_size = 1.0 / textureSize(directionallights[l].shadow_map, 0);
	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			float closestDepth = texture(directionallights[l].shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r;
			float currentDepth = proj_coords.z;
			shadow += currentDepth - bias > closestDepth ? 0.0 : 1.0;
		}
	}
	shadow /= 9.0;

	return shadow;
}
#FRAGMENT SHADER END