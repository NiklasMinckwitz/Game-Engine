#include "engine/engine.hpp"

namespace engine {
	void init() {
		window::create_window();
		graphics::init();

		glfwSetInputMode(window::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetKeyCallback(window::window, input::key_callback);
		glfwSetCursorPosCallback(window::window, input::mouse_position_callback);
		glfwSetMouseButtonCallback(window::window, input::mouse_button_callback);
	}

	void clear() {
		clear_gameobjects();
		clear_resources();
	}

	void update() {
		glfwPollEvents();
	}

	void late_update() {

	}

	void render() {
		graphics::render();
		glfwSwapBuffers(window::window);
	}

	void register_button(GLint button, GLint key) {
		input::register_button(button, key);
	}

	void unregister_button(GLint button) {
		input::unregister_button(button);
	}

	void set_mouse_sensitivity(GLfloat sensitivity) {
		input::set_mouse_sensitivity(sensitivity);
	}

	void flush_buttons() {
		input::flush_buttons();
	}

	void flush_mouse() {
		input::flush_mouse();
	}

	GLfloat get_mouse_sensitivity() {
		return input::get_mouse_sensitivity();
	}
	glm::vec2 get_mouse_position() {
		return input::get_mouse_position();
	}
	glm::vec2 get_mouse_offset() {
		return input::get_mouse_offset();
	}

	GLboolean mouse_hit(GLint button) {
		return input::mouse_hit(button);
	}
	GLboolean mouse_down(GLint button) {
		return input::mouse_down(button);
	}
	GLboolean mouse_up(GLint button) {
		return input::mouse_up(button);
	}

	bool button_hit(GLint button) {
		return input::button_hit(button);
	}
	bool button_down(GLint button) {
		return input::button_down(button);
	}
	bool button_up(GLint button) {
		return input::button_up(button);
	}

	void close() {
		glfwSetWindowShouldClose(window::window, GL_TRUE);
	}

	GLboolean should_close() {
		return glfwWindowShouldClose(window::window);
	}

	void set_resolution(GLint width, GLint height) {
		window::set_resolution(width, height);
		graphics::set_resolution(width, height);
	}

	Mesh* get_primitive(GLuint type) {
		if (type == 0) {
			return graphics::get_primitive_quad();
		}
		else if (type == 1) {
			return graphics::get_primitive_cube();
		}
		return nullptr;
	}

	GameObject* create_gameobject(glm::vec3 position, glm::quat rotation, glm::vec3 scale) {
		GameObject* object = new GameObject(position, rotation, scale);
		engine_intern::gameobjects.push_back(object);
		return object;
	}

	GameObject* create_gameobject(Transform* transform) {
		GameObject* object = new GameObject(transform);
		engine_intern::gameobjects.push_back(object);
		return object;
	}

	void clear_gameobjects() {
		for (const auto &e : engine_intern::gameobjects) {
			e->remove_all_components();
			delete e;
		}
		engine_intern::gameobjects.clear();
	}
}

namespace engine_intern {
	std::vector<GameObject*> gameobjects;
}