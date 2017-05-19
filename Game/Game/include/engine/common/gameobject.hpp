#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <vector>

#include "engine/common/transform.hpp"
#include "engine/graphics/camera.hpp"
#include "engine/graphics/meshrenderer.hpp"
#include "engine/graphics/pointlight.hpp"
#include "engine/graphics/directionallight.hpp"

class GameObject {
public:
	GameObject(glm::vec3 position = glm::vec3(0, 0, 0), glm::quat rotation = glm::quat(glm::vec3(0, 0, 0)), glm::vec3 scale = glm::vec3(1, 1, 1));
	GameObject(Transform* transform);

	Camera* add_camera_component();
	MeshRenderer* add_meshrenderer_component();
	PointLight* add_pointlight_component();
	DirectionalLight* add_directionallight_component();

	Camera* get_camera_component();
	MeshRenderer* get_meshrenderer_component();
	PointLight* get_pointlight_component();
	DirectionalLight* get_directionallight_component();

	void remove_camera_component();
	void remove_meshrenderer_component();
	void remove_pointlight_component();
	void remove_directionallight_component();

	void remove_all_components();

	Transform transform;
private:
	Camera* camera;
	MeshRenderer* meshrenderer;
	PointLight* pointlight;
	DirectionalLight* directionallight;
};

/*
The namespace components is reserved for internal functions.
*/
namespace components {
	extern std::vector<Camera*> cameras;
	extern std::vector<MeshRenderer*> meshrenderers;
	extern std::vector<PointLight*> pointlights;
	extern std::vector<DirectionalLight*> directionallights;
}

#endif