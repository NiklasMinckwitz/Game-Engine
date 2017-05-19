#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

class Transform {
public:
	void set_parent(Transform *transform);
	void add_child(Transform *transform);
	void remove_child(Transform *transform);

	void set_has_changed();

	void set_position(glm::vec3 position);
	void set_rotation(glm::quat rotation);
	void set_scale(glm::vec3 scale);

	void translate(glm::vec3 translation);
	void rotate(glm::quat rotation);

	Transform* get_parent();
	Transform* get_child(GLuint child_id);
	GLuint get_children_count();

	glm::vec3 get_position();
	glm::quat get_rotation();
	glm::vec3 get_scale();

	glm::vec3 get_world_position();
	glm::quat get_world_rotation();
	glm::vec3 get_world_scale();

	glm::vec3 get_right();
	glm::vec3 get_up();
	glm::vec3 get_forward();

	glm::mat4 get_local_to_world();
	glm::mat4 get_world_to_local();
private:
	GLboolean world_position_updated = GL_FALSE;
	GLboolean world_rotation_updated = GL_FALSE;
	GLboolean world_scale_updated = GL_FALSE;
	
	GLboolean right_updated = GL_FALSE;
	GLboolean up_updated = GL_FALSE;
	GLboolean forward_updated = GL_FALSE;

	GLboolean local_matrix_updated = GL_FALSE;
	GLboolean world_matrix_updated = GL_FALSE;

	Transform *parent;
	std::vector<Transform*> children;

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale = glm::vec3(1, 1, 1);

	glm::vec3 world_position;
	glm::quat world_rotation;
	glm::vec3 world_scale = glm::vec3(1, 1, 1);

	glm::vec3 right = glm::vec3(1, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 forward = glm::vec3(0, 0, -1);

	glm::mat4 local_to_world;
	glm::mat4 world_to_local;
};

#endif