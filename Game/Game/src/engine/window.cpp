#include "engine/window.hpp"

#include <iostream>

namespace window {
	GLFWwindow* window = nullptr;

	GLFWwindow* create_window() {
		unsigned int window_width = 800;
		unsigned int window_height = 600;
		std::string window_title = "Game";

		if (!glfwInit()) {
			std::cout << "Failed to initialize GLFW" << std::endl;
			return nullptr;
		}

		glfwWindowHint(GLFW_SAMPLES, 4); //Anitaliasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //MacOS
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		const char *cstr = window_title.c_str();
		window = glfwCreateWindow(window_width, window_height, cstr, NULL, NULL);
		if (window == NULL) {
			std::cout << "Failed to open OpenGL window" << std::endl;
			return nullptr;
		}

		glfwMakeContextCurrent(window);
		glewExperimental = true;
		if (glewInit() != GLEW_OK) {
			std::cout << "Failed to initialize GLEW" << std::endl;
			return nullptr;
		}

		return window;
	}

	void set_resolution(GLint width, GLint height) {
		glfwSetWindowSize(window, width, height);
		glViewport(0, 0, width, height);
	}
}