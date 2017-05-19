#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "engine/common/gameobject.hpp"
#include "engine/resources.hpp"

namespace graphics {
	void init();
	void init_buffers();
	
	void clear();
	void clear_buffers();

	void render();

	void set_resolution(GLuint width, GLuint height);

	Mesh* get_primitive_quad();
	Mesh* get_primitive_cube();

	void load_frame_quad();
	void load_primitive_quad();
	void load_primitive_cube();
}

#endif