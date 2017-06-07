#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>
#include <vector>

#include "engine/resources/mesh.hpp"
#include "engine/resources/shader.hpp"
#include "engine/resources/texture.hpp"
#include "engine/resources/material.hpp"

namespace engine {
	Mesh* load_mesh(std::string path);
	Shader* load_shader(std::string path);
	Texture* load_texture(std::string path);

	Material* create_material(Shader* shader);

	void clear_resources();
}

/*
The namespace resources is reserved for internal functions.
*/
namespace resources {
	extern std::vector<Mesh*> meshes;
	extern std::vector<Shader*> shaders;
	extern std::vector<Texture*> textures;
	extern std::vector<Material*> materials;
}

#endif