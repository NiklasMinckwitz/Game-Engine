#include "engine/resources/shader.hpp"

#include <fstream>
#include <iostream>

void Shader::load(std::string path) {
	//load vertex shader
	std::ifstream shader_stream(path);
	if (!shader_stream.is_open()) {
		std::cout << "Shader (" + path + ") could not be found." << std::endl;
		return;
	}

	std::string geometry_shader_code;
	std::string vertex_shader_code;
	std::string fragment_shader_code;

	std::string line = "";

	while (std::getline(shader_stream, line)) {
		if (line == "#GEOMETRY SHADER") break;
	}

	//read geometry shader
	while (std::getline(shader_stream, line)) {
		if (line == "#GEOMETRY SHADER END") break;

		geometry_shader_code += "\n" + line;
	}

	while (std::getline(shader_stream, line)) {
		if (line == "#VERTEX SHADER") break;
	}

	//read vertex shader
	while (std::getline(shader_stream, line)) {
		if (line == "#VERTEX SHADER END") break;

		vertex_shader_code += "\n" + line;
	}

	while (std::getline(shader_stream, line)) {
		if (line == "#FRAGMENT SHADER") break;
	}

	//read fragment shader
	while (std::getline(shader_stream, line)) {
		if (line == "#FRAGMENT SHADER END") break;

		fragment_shader_code += "\n" + line;
	}
	shader_stream.close();

	//compile shader
	std::cout << "Compiling: " + path << std::endl;
	if (geometry_shader_code.empty()) {
		compile(vertex_shader_code.c_str(), fragment_shader_code.c_str());
	}
	else {
		compile(vertex_shader_code.c_str(), fragment_shader_code.c_str(), geometry_shader_code.c_str());
	}
}

void Shader::compile(const GLchar *vertex_shader_code, const GLchar *fragment_shader_code, const GLchar *geometry_shader_code) {
	//Compile shaders
	GLuint geometry_shader_id;
	GLuint vertex_shader_id;
	GLuint fragment_shader_id;
	if (geometry_shader_code != nullptr)
		geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
	vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	GLint result = GL_FALSE;
	GLint info_log_length;

	//compile geometry shader
	if (geometry_shader_code != nullptr) {
		glShaderSource(geometry_shader_id, 1, &geometry_shader_code, NULL);
		glCompileShader(geometry_shader_id);
		glGetShaderiv(geometry_shader_id, GL_COMPILE_STATUS, &result);
		if (!result) {
			glGetShaderiv(geometry_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
			GLchar *info_log = new GLchar[info_log_length];
			glGetShaderInfoLog(geometry_shader_id, info_log_length, NULL, info_log);
			std::string info_log_str;
			info_log_str.append(static_cast<char *>(info_log));
			std::cout << "Vertex Shader Compile Error: " + info_log_str << std::endl;
		}
	}

	//compile vertex shader
	glShaderSource(vertex_shader_id, 1, &vertex_shader_code, NULL);
	glCompileShader(vertex_shader_id);
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
		GLchar *info_log = new GLchar[info_log_length];
		glGetShaderInfoLog(vertex_shader_id, info_log_length, NULL, info_log);
		std::string info_log_str;
		info_log_str.append(static_cast<char *>(info_log));
		std::cout << "Vertex Shader Compile Error: " + info_log_str << std::endl;
	}

	//compile fragment shader
	glShaderSource(fragment_shader_id, 1, &fragment_shader_code, NULL);
	glCompileShader(fragment_shader_id);
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
		GLchar *info_log = new GLchar[info_log_length];
		glGetShaderInfoLog(fragment_shader_id, info_log_length, NULL, info_log);
		std::string info_log_str;
		info_log_str.append(static_cast<char *>(info_log));
		std::cout << "Fragment Shader Compile Error: " + info_log_str << std::endl;
	}

	//link the program
	id = glCreateProgram();
	if (geometry_shader_code != nullptr)
		glAttachShader(id, geometry_shader_id);
	glAttachShader(id, vertex_shader_id);
	glAttachShader(id, fragment_shader_id);
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (!result) {
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &info_log_length);
		GLchar *info_log = new GLchar[info_log_length];
		glGetShaderInfoLog(id, info_log_length, NULL, info_log);
		std::string info_log_str;
		info_log_str.append(static_cast<char *>(info_log));
		std::cout << "Shader Program Compile Error: " + info_log_str << std::endl;
	}

	GLint uniform_count;
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniform_count);

	GLint max_name_length = 128;
	GLint size;
	GLenum type;
	GLsizei length;
	GLchar *name = new GLchar[max_name_length];

	for (int i = 0; i < uniform_count; i++) {
		glGetActiveUniform(id, (GLuint)i, max_name_length, &length, &size, &type, name);
		locations[name] = i;
		location_types.push_back(type);
	}
	delete[] name;

	if (geometry_shader_code != nullptr)
		glDetachShader(id, geometry_shader_id);
	glDetachShader(id, vertex_shader_id);
	glDetachShader(id, fragment_shader_id);

	if (geometry_shader_code != nullptr)
		glDeleteShader(geometry_shader_id);
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);
}

GLuint Shader::get_id() {
	return id;
}

GLuint Shader::get_location(std::string location_name) {
	return locations[location_name];
}

GLuint Shader::get_location_type(GLuint location) {
	return location_types[location];
}