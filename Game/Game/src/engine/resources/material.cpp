#include "engine/resources/material.hpp"

#include <iostream>

Material::Material(Shader* geometry_shader) {
	this->geometry_shader = geometry_shader;
}

void Material::set_float(std::string uniform_string, GLfloat value) {
	floats[uniform_string] = value;
}

void Material::set_vec2(std::string uniform_string, glm::vec2 value) {
	vec2s[uniform_string] = value;
}

void Material::set_vec3(std::string uniform_string, glm::vec3 value) {
	vec3s[uniform_string] = value;
}

void Material::set_mat3(std::string uniform_string, glm::mat3 value) {
	mat3s[uniform_string] = value;
}

void Material::set_mat4(std::string uniform_string, glm::mat4 value) {
	mat4s[uniform_string] = value;
}

void Material::set_texture(std::string uniform_string, Texture* texture) {
	textures[uniform_string] = texture;
}

void Material::use() {
	glUseProgram(geometry_shader->get_id());

	for (auto const& v : floats) {
		glUniform1f(geometry_shader->get_location(v.first), v.second);
	}

	for (auto const& v : vec2s) {
		glUniform2f(geometry_shader->get_location(v.first), v.second.x, v.second.y);
	}

	for (auto const& v : vec3s) {
		glUniform3f(geometry_shader->get_location(v.first), v.second.x, v.second.y, v.second.z);
	}

	for (auto const& v : mat3s) {
		glUniformMatrix3fv(geometry_shader->get_location(v.first), 1, GL_FALSE, &v.second[0][0]);
	}

	for (auto const& v : mat4s) {
		glUniformMatrix4fv(geometry_shader->get_location(v.first), 1, GL_FALSE, &v.second[0][0]);
	}

	GLuint current_texture = 0;
	for (auto const& v : textures) {
		glActiveTexture(GL_TEXTURE0 + current_texture);
		glBindTexture(GL_TEXTURE_2D, v.second->get_id());
		glUniform1i(geometry_shader->get_location(v.first), current_texture);
		current_texture++;
	}
}

void Material::unuse() {
	GLuint current_texture = 0;
	for (auto const& v : textures) {
		glActiveTexture(GL_TEXTURE0 + current_texture);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(geometry_shader->get_location(v.first), current_texture);
		current_texture++;
	}
}

Shader* Material::get_shader() {
	return geometry_shader;
}