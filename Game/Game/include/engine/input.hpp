#ifndef INPUT_HPP
#define INPUT_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>

namespace input {
	struct button_state {
		GLint key;
		GLboolean hit;
		GLboolean down;
		GLboolean up;
	};

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
	void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);

	void set_mouse_sensitivity(GLfloat sensitivity);
	GLfloat get_mouse_sensitivity();
	glm::vec2 get_mouse_position();
	glm::vec2 get_mouse_offset();

	void register_button(GLint button, GLint key);
	void unregister_button(GLint button);
	void flush_buttons();
	void flush_mouse();

	GLboolean mouse_hit(GLint button);
	GLboolean mouse_down(GLint button);
	GLboolean mouse_up(GLint button);

	GLboolean button_hit(GLint button);
	GLboolean button_down(GLint button);
	GLboolean button_up(GLint button);
}

#endif