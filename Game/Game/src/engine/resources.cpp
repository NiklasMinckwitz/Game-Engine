#include "engine/resources.hpp"

namespace engine {
	Mesh* load_mesh(std::string path) {
		Mesh* mesh = new Mesh();
		mesh->load(path);
		resources::meshes.push_back(mesh);
		return mesh;
	}

	Shader* load_shader(std::string path) {
		Shader* shader = new Shader();
		shader->load(path);
		resources::shaders.push_back(shader);
		return shader;
	}

	Texture* load_texture(std::string path) {
		Texture* texture = new Texture();
		texture->load(path);
		resources::textures.push_back(texture);
		return texture;
	}

	Material* create_material(Shader* shader) {
		Material* material = new Material(shader);
		resources::materials.push_back(material);
		return material;
	}

	void clear_resources() {
		for (const auto &e : resources::meshes) {
			delete e;
		}
		for (const auto &e : resources::shaders) {
			delete e;
		}
		for (const auto &e : resources::textures) {
			delete e;
		}
		for (const auto &e : resources::materials) {
			delete e;
		}
		resources::meshes.clear();
		resources::shaders.clear();
		resources::textures.clear();
		resources::materials.clear();
	}
}

namespace resources {
	std::vector<Mesh*> meshes;
	std::vector<Shader*> shaders;
	std::vector<Texture*> textures;
	std::vector<Material*> materials;
}