#include "engine/graphics/camera.hpp"

#include <glm/gtx/transform.hpp>

Camera::Camera(Transform* transform) {
	this->transform = transform;
}

void Camera::set_exposure(GLfloat exposure) {
	this->exposure = exposure;
}

void Camera::set_field_of_view(GLfloat field_of_view) {
	this->field_of_view = field_of_view;
	proj_matrix_updated = GL_FALSE;
}
void Camera::set_aspect_ratio(GLfloat aspect_ratio) {
	this->aspect_ratio = aspect_ratio;
	proj_matrix_updated = GL_FALSE;
}
void Camera::set_near_plane(GLfloat near_plane) {
	this->near_plane = near_plane;
	proj_matrix_updated = GL_FALSE;
}
void Camera::set_far_plane(GLfloat far_plane) {
	this->far_plane = far_plane;
	proj_matrix_updated = GL_FALSE;
}

glm::vec3 Camera::get_position() {
	return transform->get_world_position();
}

GLfloat Camera::get_exposure() {
	return exposure;
}

glm::mat4 Camera::get_proj_matrix() {
	if (!proj_matrix_updated) {
		projection_matrix = glm::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
		proj_matrix_updated = GL_TRUE;
	}
	return projection_matrix;
}

/*
TODO: If possible, update view_matrix only when the transform has changed
(Maybe observer pattern on transform)
*/

glm::mat4 Camera::get_view_matrix() {
	return glm::lookAt(transform->get_world_position(), transform->get_world_position() + transform->get_forward(), transform->get_up());
}