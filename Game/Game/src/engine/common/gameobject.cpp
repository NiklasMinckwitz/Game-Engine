#include "engine/common/gameobject.hpp"

#include <iostream>

GameObject::GameObject(glm::vec3 position, glm::quat rotation, glm::vec3 scale) {
	transform.set_position(position);
	transform.set_rotation(rotation);
	transform.set_scale(scale);
}

GameObject::GameObject(Transform* transform) {
	this->transform.set_position(transform->get_position());
	this->transform.set_rotation(transform->get_rotation());
	this->transform.set_scale(transform->get_scale());
}

Camera* GameObject::add_camera_component() {
	if (camera != nullptr) {
		std::cout << "A gameobject can only have 1 camera component" << std::endl;
		return nullptr;
	}
	camera = new Camera(&transform);
	components::cameras.push_back(camera);
	return camera;
}

MeshRenderer* GameObject::add_meshrenderer_component() {
	if (meshrenderer != nullptr) {
		std::cout << "A gameobject can only have 1 meshrenderer component" << std::endl;
		return nullptr;
	}
	meshrenderer = new MeshRenderer(&transform);
	components::meshrenderers.push_back(meshrenderer);
	return meshrenderer;
}

PointLight* GameObject::add_pointlight_component() {
	if (pointlight != nullptr) {
		std::cout << "A gameobject can only have 1 pointlight component" << std::endl;
		return nullptr;
	}
	pointlight = new PointLight(&transform);
	components::pointlights.push_back(pointlight);
	return pointlight;
}

DirectionalLight* GameObject::add_directionallight_component() {
	if (directionallight != nullptr) {
		std::cout << "A gameobject can only have 1 directionallight component" << std::endl;
		return nullptr;
	}
	directionallight = new DirectionalLight(&transform);
	components::directionallights.push_back(directionallight);
	return directionallight;
}

Camera* GameObject::get_camera_component() {
	return camera;
}
MeshRenderer* GameObject::get_meshrenderer_component() {
	return meshrenderer;
}
PointLight* GameObject::get_pointlight_component() {
	return pointlight;
}
DirectionalLight* GameObject::get_directionallight_component() {
	return directionallight;
}

void GameObject::remove_camera_component() {
	if (camera == nullptr) return;
	auto &position = std::find(components::cameras.begin(), components::cameras.end(), camera);
	if (position == components::cameras.end()) std::cout << "is end" << std::endl;
	std::iter_swap(position, components::cameras.end() - 1);
	components::cameras.pop_back();
	delete camera;
	camera = nullptr;
}

void GameObject::remove_meshrenderer_component() {
	if (meshrenderer == nullptr) return;
	auto &position = std::find(components::meshrenderers.begin(), components::meshrenderers.end(), meshrenderer);
	if (position == components::meshrenderers.end()) std::cout << "is end" << std::endl;
	std::iter_swap(position, components::meshrenderers.end() - 1);
	components::meshrenderers.pop_back();
	delete meshrenderer;
	meshrenderer = nullptr;
}

void GameObject::remove_pointlight_component() {
	if (pointlight == nullptr) return;
	auto &position = std::find(components::pointlights.begin(), components::pointlights.end(), pointlight);
	if (position == components::pointlights.end()) std::cout << "is end" << std::endl;
	std::iter_swap(position, components::pointlights.end() - 1);
	components::pointlights.pop_back();
	delete pointlight;
	pointlight = nullptr;
}

void GameObject::remove_directionallight_component() {
	if (directionallight == nullptr) return;
	auto &position = std::find(components::directionallights.begin(), components::directionallights.end(), directionallight);
	if (position == components::directionallights.end()) std::cout << "is end" << std::endl;
	std::iter_swap(position, components::directionallights.end() - 1);
	components::directionallights.pop_back();
	delete directionallight;
	directionallight = nullptr;
}

void GameObject::remove_all_components() {
	remove_camera_component();
	remove_meshrenderer_component();
	remove_pointlight_component();
	remove_directionallight_component();
}

namespace components {
	std::vector<Camera*> cameras;
	std::vector<MeshRenderer*> meshrenderers;
	std::vector<PointLight*> pointlights;
	std::vector<DirectionalLight*> directionallights;
}