#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "engine/common/transform.hpp"

class PointLight {
public:
	PointLight(Transform* transform);

	void set_color(glm::vec3 color);
	void set_intensity(GLfloat value);
	void set_range(GLfloat range);

	glm::vec3 get_color();
	glm::vec3 get_position();
	GLfloat get_intensity();

	GLfloat get_constant();
	GLfloat get_linear();
	GLfloat get_quadratic();

private:
	Transform* transform;

	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	GLfloat intensity = 1.0f;

	GLfloat constant = 1.0f;
	GLfloat linear = 0.07f;
	GLfloat quadratic = 0.017f;
};

#endif