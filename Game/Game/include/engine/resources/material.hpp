#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <unordered_map>

#include "engine/resources/shader.hpp"
#include "engine/resources/texture.hpp"

class Material {
public:
	Material(Shader* geometry_shader);

	void set_float(std::string uniform_string, GLfloat value);
	void set_vec2(std::string uniform_string, glm::vec2 value);
	void set_vec3(std::string uniform_string, glm::vec3 value);
	void set_mat3(std::string uniform_string, glm::mat3 value);
	void set_mat4(std::string uniform_string, glm::mat4 value);
	void set_texture(std::string uniform_string, Texture* texture);

	void use();
	void unuse();

	Shader* get_shader();
private:
	Shader* geometry_shader;

	std::unordered_map<std::string, GLfloat> floats;
	std::unordered_map<std::string, glm::vec2> vec2s;
	std::unordered_map<std::string, glm::vec3> vec3s;
	std::unordered_map<std::string, glm::mat3> mat3s;
	std::unordered_map<std::string, glm::mat4> mat4s;
	std::unordered_map<std::string, Texture*> textures;
};

#endif