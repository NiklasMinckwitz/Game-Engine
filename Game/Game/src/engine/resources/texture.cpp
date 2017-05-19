#include "engine/resources/texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <engine/libraries/stb_image.h>
#include <iostream>
#include <fstream>

void Texture::create(GLint format, GLuint width, GLuint height) {
	format = format;
	width = width;
	height = height;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::load(std::string path) {
	int width, height, comp;
	const char* c_file_path = path.c_str();
	stbi_uc *image = stbi_load(c_file_path, &width, &height, &comp, STBI_rgb_alpha);
	if (!image) {
		std::cout << "Texture (" + path + ") could not be found." << std::endl;
		return;
	}

	this->width = width;
	this->height = height;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	if (comp == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		format = GL_RGB;
	}
	else if (comp == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		format = GL_RGBA;
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::get_id() {
	return id;
}

GLint Texture::get_format() {
	return format;
}

GLuint Texture::get_width() {
	return width;
}

GLuint Texture::get_height() {
	return height;
}