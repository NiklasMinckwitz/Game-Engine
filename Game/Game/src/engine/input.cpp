#include "engine/input.hpp"

namespace input {
	std::map<GLint, GLint> keys;
	std::map<GLint, button_state> buttons;

	GLboolean mouse_hit_buttons[3];
	GLboolean mouse_down_buttons[3];
	GLboolean mouse_up_buttons[3];

	GLfloat mouse_sensitivity = 0.05f;
	glm::vec2 mouse_position;
	glm::vec2 mouse_offset;
	glm::vec2 old_mouse_position;

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
		if (keys.count(key) > 0) {
			int button = keys[key];

			if (action == GLFW_PRESS) {
				if (!buttons[button].down) buttons[button].hit = GL_TRUE;
				buttons[button].down = GL_TRUE;
			}
			else if (action == GLFW_RELEASE) {
				if (buttons[button].down) buttons[button].up = GL_TRUE;
				buttons[button].down = GL_FALSE;
			}
		}
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
		if (action == GLFW_PRESS) {
			if (!mouse_down_buttons[button]) mouse_hit_buttons[button] = GL_TRUE;
			mouse_down_buttons[button] = GL_TRUE;
		} else if (action == GLFW_RELEASE) {
			if (mouse_down_buttons[button]) mouse_up_buttons[button] = GL_TRUE;
			mouse_down_buttons[button] = GL_FALSE;
		}
	}

	void mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
		mouse_position = glm::vec2(xpos, ypos);

		mouse_offset = mouse_sensitivity * glm::vec2(xpos - old_mouse_position.x, old_mouse_position.y - ypos);

		old_mouse_position = mouse_position;
	}

	void set_mouse_sensitivity(GLfloat sensitivity) {
		mouse_sensitivity = sensitivity;
	}

	GLfloat get_mouse_sensitivity() {
		return mouse_sensitivity;
	}

	glm::vec2 get_mouse_position() {
		return mouse_position;
	}

	glm::vec2 get_mouse_offset() {
		glm::vec2 result = mouse_offset;
		mouse_offset = glm::vec2(0, 0);
		return result;
	}

	void register_button(GLint button, GLint key) {
		input::button_state state;
		state.key = key;
		state.hit = GL_FALSE;
		state.down = GL_FALSE;
		state.up = GL_FALSE;
		input::keys.insert(std::make_pair(key, button));
		input::buttons.insert(std::make_pair(button, state));
	}

	void unregister_button(GLint button) {
		if (input::buttons.count(button) > 0) {
			int key = input::buttons[button].key;
			input::keys.erase(key);
			input::buttons.erase(button);
		}
	}

	void flush_buttons() {
		for (std::pair<GLint, input::button_state> element : input::buttons) {
			element.second.hit = GL_FALSE;
			element.second.up = GL_FALSE;
		}
	}

	void flush_mouse() {
		for (GLint i = 0; i < 3; ++i) {
			mouse_hit_buttons[i] = GL_FALSE;
			mouse_down_buttons[i] = GL_FALSE;
			mouse_up_buttons[i] = GL_FALSE;
		}
	}

	GLboolean mouse_hit(GLint button) {
		GLboolean result = mouse_hit_buttons[button];
		mouse_hit_buttons[button] = GL_FALSE;
		return result;
	}

	GLboolean mouse_down(GLint button) {
		return mouse_down_buttons[button];
	}

	GLboolean mouse_up(GLint button) {
		GLboolean result = mouse_up_buttons[button];
		mouse_up_buttons[button] = GL_FALSE;
		return result;
	}

	GLboolean button_hit(GLint button) {
		GLboolean result = input::buttons[button].hit;
		buttons[button].hit = GL_FALSE;
		return result;
	}

	GLboolean button_down(GLint button) {
		return buttons[button].down;
	}

	GLboolean button_up(GLint button) {
		GLboolean result = input::buttons[button].up;
		buttons[button].up = GL_FALSE;
		return result;
	}
}
