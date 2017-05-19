#ifndef MESHRENDERER_HPP
#define MESHRENDERER_HPP

#include "engine/common/transform.hpp"
#include "engine/graphics/camera.hpp"
#include "engine/resources/mesh.hpp"
#include "engine/resources/material.hpp"

class MeshRenderer {
public:
	MeshRenderer(Transform* transform);

	void render_geometry(Camera* camera, Shader* shader);
	void render_depth(Shader* shader);

	Mesh* mesh;
	Material* material;
private:
	Transform* transform;
};

#endif