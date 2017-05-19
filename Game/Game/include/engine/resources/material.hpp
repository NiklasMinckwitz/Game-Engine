#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <unordered_map>

#include "engine/resources/shader.hpp"
#include "engine/resources/texture.hpp"

class Material {
public:
	Texture* albedo = nullptr;
	Texture* specular = nullptr;
	Texture* normal = nullptr;

private:

};

#endif