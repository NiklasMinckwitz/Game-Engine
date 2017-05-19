#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "engine/common/transform.hpp"

class Camera {
public:
	Camera(Transform* transform);

	void set_exposure(GLfloat exposure);

	void set_field_of_view(GLfloat field_of_view);
	void set_aspect_ratio(GLfloat aspect_ratio);
	void set_near_plane(GLfloat near_plane);
	void set_far_plane(GLfloat far_plane);

	glm::vec3 get_position();

	GLfloat get_exposure();

	glm::mat4 get_proj_matrix();
	glm::mat4 get_view_matrix();

private:
	Transform* transform;

	GLboolean proj_matrix_updated = GL_FALSE;

	GLfloat exposure = 1.0f;

	GLfloat field_of_view = 45.0f;
	GLfloat aspect_ratio = 800.0f / 600.0f;
	GLfloat near_plane = 0.01f;
	GLfloat far_plane = 100.0f;

	glm::mat4 projection_matrix;
};

#endif