#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace window {
	extern GLFWwindow* window;

	GLFWwindow* create_window();

	void set_resolution(GLint width, GLint height);
}

#endif