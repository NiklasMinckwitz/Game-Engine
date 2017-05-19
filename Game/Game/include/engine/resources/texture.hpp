#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <string>

class Texture {
public:
	void create(GLint format, GLuint width, GLuint height);
	void load(std::string path);

	GLuint get_id();
	GLint get_format();
	GLuint get_width();
	GLuint get_height();
private:
	GLuint id;

	GLint format;
	GLuint width;
	GLuint height;
};

#endif