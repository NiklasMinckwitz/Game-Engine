#include "engine/common/transform.hpp"

#include <glm/gtx/transform.hpp>

void Transform::set_parent(Transform *transform) {
	//remove this transform from former parent children
	if (parent != nullptr) {
		parent->remove_child(this);
	}

	//set new parent
	parent = transform;

	//add this transform to new parent children
	if (parent != nullptr) {
		parent->add_child(this);
	}
	
	set_has_changed();
}

void Transform::add_child(Transform *transform) {
	//return if new child is nullptr
	if (transform == nullptr) return;

	//add new child to children vector
	children.push_back(transform);

	//set new child has_changed to true
	transform->set_has_changed();
}

void Transform::remove_child(Transform *transform) {
	//return if there is no child to remove
	if (transform == nullptr) return;

	//try to find the child in children
	GLuint element;
	for (element = 0; element < children.size(); ++element) {
		if (children[element] == transform) {
			break;
		}
	}

	//if it was found remove it else there is nothing to remove
	if (element == children.size()) return;
	children.erase(children.begin() + element);

	//set removed child has_changed to true
	transform->set_has_changed();
}

void Transform::set_has_changed() {
	//if has_changed is already true, return
	if (!world_position_updated &&
		!world_rotation_updated &&
		!world_scale_updated &&
		!right_updated &&
		!up_updated &&
		!forward_updated &&
		!local_matrix_updated &&
		!world_matrix_updated) return;

	//set has_changed true
	world_position_updated = GL_FALSE;
	world_rotation_updated = GL_FALSE;
	world_scale_updated = GL_FALSE;
	right_updated = GL_FALSE;
	up_updated = GL_FALSE;
	forward_updated = GL_FALSE;
	local_matrix_updated = GL_FALSE;
	world_matrix_updated = GL_FALSE;

	//set has_changed true for each child
	for (GLuint i = 0; i < children.size(); ++i) {
		children[i]->set_has_changed();
	}
}

void Transform::set_position(glm::vec3 position) {
	//set local position
	this->position = position;
	//set has_changed to true
	set_has_changed();
}

void Transform::set_rotation(glm::quat rotation) {
	//set local rotation
	this->rotation = rotation;
	//set has_changed to true
	set_has_changed();
}

void Transform::set_scale(glm::vec3 scale) {
	//set local scale
	this->scale = scale;
	//set has_changed to true
	set_has_changed();
}

void Transform::translate(glm::vec3 translation) {
	//translate local position
	set_position(position + translation);
}

void Transform::rotate(glm::quat rotation) {
	//rotate local rotation
	set_rotation(this->rotation * rotation);
}

Transform* Transform::get_parent() {
	//return parent
	return parent;
}

Transform* Transform::get_child(GLuint child_id) {
	//return child at position child_id
	return children[child_id];
}

GLuint Transform::get_children_count() {
	//return child count
	return children.size();
}

glm::vec3 Transform::get_position() {
	//return local position
	return position;
}

glm::quat Transform::get_rotation() {
	//return local rotation
	return rotation;
}

glm::vec3 Transform::get_scale() {
	//return local scale
	return scale;
}

glm::vec3 Transform::get_world_position() {
	//check if world_position is already up to date
	//if not, update world_position
	if (!world_position_updated) {
		if (parent == nullptr) {
			world_position = position;
		}
		else {
			world_position = glm::vec4(position, 1) * get_local_to_world();
		}
		world_position_updated = GL_TRUE;
	}
	return world_position;
}

glm::quat Transform::get_world_rotation() {
	//check if world_rotation is already up to date
	//if not, update world_rotation
	if (!world_rotation_updated) {
		if (parent == nullptr) {
			world_rotation = rotation;
		}
		else {
			world_rotation = rotation * parent->get_world_rotation();
		}
		world_rotation_updated = GL_TRUE;
	}
	return world_rotation;
}

glm::vec3 Transform::get_world_scale() {
	//check if world_scale is already up to date
	//if not, update world_scale
	if (!world_scale_updated) {
		if (parent == nullptr) {
			world_scale = scale;
		}
		else {
			world_scale = scale * parent->get_world_scale();
		}
		world_scale_updated = GL_TRUE;
	}
	return world_scale;
}

glm::vec3 Transform::get_right() {
	//check if right is already up to date
	//if not, update right
	if (!right_updated) {
		right = glm::normalize(glm::vec3(1, 0, 0) * glm::mat3_cast(get_world_rotation()));
	}
	return right;
}

glm::vec3 Transform::get_up() {
	//check if up is already up to date
	//if not, update up
	if (!up_updated) {
		up = glm::normalize(glm::vec3(0, 1, 0) * glm::mat3_cast(get_world_rotation()));
	}
	return up;
}

glm::vec3 Transform::get_forward() {
	//check if forward is already up to date
	//if not, update forward
	if (!forward_updated) {
		forward = glm::normalize(glm::vec3(0, 0, -1) * glm::mat3_cast(get_world_rotation()));
	}
	return forward;
}

glm::mat4 Transform::get_local_to_world() {
	//check if local_to_world is already up to date
	//if not, update local_to_world
	if (!local_matrix_updated) {
		local_to_world = glm::translate(position) * glm::mat4_cast(rotation) * glm::scale(scale);
		if (parent != nullptr) {
			local_to_world = parent->get_local_to_world() * local_to_world;
		}
		local_matrix_updated = GL_TRUE;
	}
	return local_to_world;
}

glm::mat4 Transform::get_world_to_local() {
	//check if world_to_local is already up to date
	//if not, update world_to_local
	if (!world_matrix_updated) {
		world_to_local = glm::inverse(get_local_to_world());
		world_matrix_updated = GL_TRUE;
	}
	return world_to_local;
}