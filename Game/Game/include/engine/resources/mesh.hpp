#ifndef MESH_HPP
#define MESH_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Mesh {
public:
	void load(std::string path);
	void calculate_tbm();
	void apply();

	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> tangents;

	GLuint vao;		//vertex array object
	GLuint vbo;		//vertex buffer object
	GLuint ibo;		//index buffer object
	GLuint nbo;		//normal buffer object
	GLuint uvbo;	//uv buffer object
	GLuint tbo;		//tangent buffer object
private:

};

#endif