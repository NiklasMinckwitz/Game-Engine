#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <unordered_map>
#include <vector>
#include <string>

class Shader {
public:
	void load(std::string path);
	void compile(const GLchar *vertex_shader_code, const GLchar *fragment_shader_code, const GLchar *geometry_shader_code = nullptr);

	GLuint get_id();
	GLuint get_location(std::string location_name);
	GLuint get_location_type(GLuint location);

private:
	GLuint id;
	std::unordered_map<std::string, GLuint> locations;
	std::vector<GLenum> location_types;
};

#endif