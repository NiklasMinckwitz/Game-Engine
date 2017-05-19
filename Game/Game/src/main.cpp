#include "engine/engine.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

#define ESCAPE_BUTTON 1

#define UP_BUTTON 100
#define LEFT_BUTTON 101
#define RIGHT_BUTTON 102
#define DOWN_BUTTON 103

int main() {
	//init engines window, graphics, input
	engine::init();
	engine::set_resolution(1024, 768);
	
	engine::set_mouse_sensitivity(0.2f);

	engine::register_button(ESCAPE_BUTTON, GLFW_KEY_ESCAPE);

	engine::register_button(UP_BUTTON, GLFW_KEY_W);
	engine::register_button(UP_BUTTON, GLFW_KEY_UP);
	engine::register_button(LEFT_BUTTON, GLFW_KEY_A);
	engine::register_button(LEFT_BUTTON, GLFW_KEY_LEFT);
	engine::register_button(RIGHT_BUTTON, GLFW_KEY_D);
	engine::register_button(RIGHT_BUTTON, GLFW_KEY_RIGHT);
	engine::register_button(DOWN_BUTTON, GLFW_KEY_S);
	engine::register_button(DOWN_BUTTON, GLFW_KEY_DOWN);

	//load resources
	Texture* octostone_albedo = engine::load_texture("res/textures/octostone/octostoneAlbedo.png");
	Texture* octostone_metallic = engine::load_texture("res/textures/octostone/octostoneRoughness.png");
	Texture* octostone_normal = engine::load_texture("res/textures/octostone/octostoneNormalc.png");

	Texture* rubber_albedo = engine::load_texture("res/textures/rubber/albedo.png");
	Texture* rubber_metallic = engine::load_texture("res/textures/rubber/roughness.png");
	Texture* rubber_normal = engine::load_texture("res/textures/rubber/normal.png");

	Texture* dirt_albedo = engine::load_texture("res/textures/dirt/albedo.png");
	Texture* dirt_metallic = engine::load_texture("res/textures/dirt/roughness.png");
	Texture* dirt_normal = engine::load_texture("res/textures/dirt/normal.png");

	Material* mat_octostone = engine::create_material();
	mat_octostone->albedo = octostone_albedo;
	mat_octostone->specular = octostone_metallic;
	mat_octostone->normal = octostone_normal;

	Material* mat_rubber = engine::create_material();
	mat_rubber->albedo = rubber_albedo;
	mat_rubber->specular = rubber_metallic;
	mat_rubber->normal = rubber_normal;

	Material* mat_dirt = engine::create_material();
	mat_dirt->albedo = dirt_albedo;
	mat_dirt->specular = dirt_metallic;
	mat_dirt->normal = dirt_normal;

	//Mesh* container_mesh = engine::load_mesh("res/models/container1.obj");
	//container_mesh->calculate_tbm();

	//create scene
	GameObject* camera1 = engine::create_gameobject();
	{
		Camera* camera_comp = camera1->add_camera_component();
		camera_comp->set_aspect_ratio(800.0f / 600.0f);
		camera_comp->set_exposure(1.0f);
		camera1->transform.set_position(glm::vec3(0.0f, 0.25f, 3.0f));
	}

	GameObject* dir_light1 = engine::create_gameobject();
	{
		DirectionalLight* dir_light = dir_light1->add_directionallight_component();
		dir_light1->transform.set_position(glm::vec3(10.0f, 10.0f, 10.0f));
		dir_light1->transform.set_rotation(glm::quat(glm::vec3(glm::radians(50.0f), glm::radians(-30.0f), 0)));
		dir_light1->transform.set_scale(glm::vec3(0.1f, 0.1f, 0.2f));

		dir_light->set_intensity(10.0f);
		dir_light->set_color(glm::vec3(1.0f, 0.8f, 0.6f));
		dir_light->set_cast_shadows(GL_TRUE);
	}

	GameObject* test_quad = engine::create_gameobject();
	{
		MeshRenderer* mesh_renderer = test_quad->add_meshrenderer_component();
		mesh_renderer->material = mat_dirt;
		mesh_renderer->mesh = engine::get_primitive(0);
		test_quad->transform.set_position(glm::vec3(0, 0, 0));
		test_quad->transform.set_rotation(glm::quat(glm::vec3(glm::radians(-90.0f), 0, 0)));
		test_quad->transform.set_scale(glm::vec3(2, 2, 2));
	}

	GameObject* test_cube = engine::create_gameobject();
	{
		MeshRenderer* mesh_renderer = test_cube->add_meshrenderer_component();
		mesh_renderer->material = mat_rubber;
		mesh_renderer->mesh = engine::get_primitive(1);
		test_cube->transform.set_position(glm::vec3(0, 0.3f, 0));
		test_cube->transform.set_scale(glm::vec3(0.25f, 0.25f, 0.25f));
	}

	GameObject* test_cube2 = engine::create_gameobject();
	{
		MeshRenderer* mesh_renderer = test_cube2->add_meshrenderer_component();
		mesh_renderer->material = mat_rubber;
		mesh_renderer->mesh = engine::get_primitive(1);
		test_cube2->transform.set_position(glm::vec3(0.75f, 0.0f, 0));
		test_cube2->transform.set_scale(glm::vec3(0.1f, 0.1f, 0.1f));
		test_cube2->transform.set_parent(&(test_cube->transform));
	}

	//variables
	const GLfloat camera_speed = 0.1f;
	GLfloat yaw = 0, pitch = 0;

	//main loop
	while (!engine::should_close()) {
		engine::update();

		if (engine::button_hit(ESCAPE_BUTTON)) {
			engine::close();
		}

		test_quad->transform.rotate(glm::quat(glm::vec3(0, 0, glm::radians(1.0f))));
		test_cube->transform.rotate(glm::quat(glm::vec3(0, glm::radians(1.0f), glm::radians(1.0f))));

		glm::vec2 mouse_off = engine::get_mouse_offset();
		yaw += mouse_off.x;
		pitch -= mouse_off.y;
		pitch = glm::clamp(pitch, -90.0f, 90.0f);
		camera1->transform.set_rotation(glm::quat(glm::vec3(0, glm::radians(yaw), 0)));

		if (engine::button_down(UP_BUTTON)) {
			camera1->transform.translate(camera1->transform.get_forward() * camera_speed);
		}
		else if (engine::button_down(DOWN_BUTTON)) {
			camera1->transform.translate(-camera1->transform.get_forward() * camera_speed);
		}

		if (engine::button_down(LEFT_BUTTON)) {
			camera1->transform.translate(-camera1->transform.get_right() * camera_speed);
		}
		else if (engine::button_down(RIGHT_BUTTON)) {
			camera1->transform.translate(camera1->transform.get_right() * camera_speed);
		}

		engine::late_update();
		engine::render();
	}

	//clear all memory allocations
	engine::clear();
}