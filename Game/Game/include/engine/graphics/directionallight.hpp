#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "engine/common/transform.hpp"
#include "engine/resources/shader.hpp"

class DirectionalLight {
public:
	DirectionalLight(Transform* transform);

	void use_shadow_buffer(Shader* shader);

	void set_cast_shadows(GLboolean value);
	void set_color(glm::vec3 color);
	void set_intensity(GLfloat value);

	GLboolean get_cast_shadows();
	glm::vec3 get_color();
	glm::vec3 get_direction();
	GLfloat get_intensity();
	GLuint get_shadow_map();
	glm::mat4 get_light_view();
	glm::mat4 get_light_projection();
	static const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
private:
	Transform* transform;

	GLboolean cast_shadows = GL_FALSE;
	GLuint depth_frame_buffer;
	GLuint depth_map;

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	GLfloat intensity = 1.0f;

	glm::mat4 projection;
};

#endif