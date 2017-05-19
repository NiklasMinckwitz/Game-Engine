#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "engine/resources.hpp"
#include "engine/window.hpp"
#include "engine/graphics.hpp"
#include "engine/input.hpp"

#include "engine/common/gameobject.hpp"

namespace engine {
	void init();
	void clear();

	void update();
	void late_update();
	void render();

	void register_button(GLint button, GLint key);
	void unregister_button(GLint button);
	void set_mouse_sensitivity(GLfloat sensitivity);
	void flush_buttons();
	void flush_mouse();

	GLfloat get_mouse_sensitivity();
	glm::vec2 get_mouse_position();
	glm::vec2 get_mouse_offset();

	GLboolean mouse_hit(GLint button);
	GLboolean mouse_down(GLint button);
	GLboolean mouse_up(GLint button);

	bool button_hit(GLint button);
	bool button_down(GLint button);
	bool button_up(GLint button);

	void close();
	GLboolean should_close();

	void set_resolution(GLint width, GLint height);

	Mesh* get_primitive(GLuint type);

	GameObject* create_gameobject(glm::vec3 position = glm::vec3(0, 0, 0), glm::quat rotation = glm::quat(glm::vec3(0, 0, 0)), glm::vec3 scale = glm::vec3(1, 1, 1));
	GameObject* create_gameobject(Transform* transform);

	void clear_gameobjects();
}

/*
The namespace engine_intern is reserved for internal functions.
*/
namespace engine_intern {
	extern std::vector<GameObject*> gameobjects;
}

#endif