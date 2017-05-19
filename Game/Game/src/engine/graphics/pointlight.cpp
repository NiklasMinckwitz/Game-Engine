#include "engine/graphics/pointlight.hpp"

PointLight::PointLight(Transform* transform) {
	this->transform = transform;
}

void PointLight::set_color(glm::vec3 color) {
	this->color = color;
}

void PointLight::set_intensity(GLfloat value) {
	intensity = value;
}

void PointLight::set_range(GLfloat range) {
	constant = 1.0f;
	linear = 4.6905f / range;
	quadratic = 82.44478f / (range * range);
}

glm::vec3 PointLight::get_color() {
	return color;
}

glm::vec3 PointLight::get_position() {
	return transform->get_world_position();
}

GLfloat PointLight::get_intensity() {
	return intensity;
}

GLfloat PointLight::get_constant() {
	return constant;
}
GLfloat PointLight::get_linear() {
	return linear;
}
GLfloat PointLight::get_quadratic() {
	return quadratic;
}