#include "engine/graphics.hpp"

#include <iostream>

namespace graphics {
	GLuint res_width = 800, res_height = 600;

	GLuint frame_buffer;
	GLuint frame_buffer_texture;

	GLuint g_buffer;
	GLuint position_texture;
	GLuint normals_texture;
	GLuint albedo_specular_texture;
	GLuint normal_map_texture;
	GLuint tangents_texture;

	Shader* frame_buffer_shader;
	Shader* g_buffer_shader;
	Shader* lightning_shader;
	Shader* depth_shader;

	Mesh* frame_quad = nullptr;
	Mesh* primitive_quad = nullptr;
	Mesh* primitive_cube = nullptr;

	void init() {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		init_buffers();
		frame_buffer_shader = engine::load_shader("res/shaders/frame.shader");
		g_buffer_shader = engine::load_shader("res/shaders/geometry.shader");
		lightning_shader = engine::load_shader("res/shaders/lightning.shader");
		depth_shader = engine::load_shader("res/shaders/depth.shader");

		load_frame_quad();
		load_primitive_quad();
		load_primitive_cube();
	}

	void init_buffers() {
		//generate frame buffer
		glGenFramebuffers(1, &frame_buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

		//generate frame_buffer texture
		glGenTextures(1, &frame_buffer_texture);
		glBindTexture(GL_TEXTURE_2D, frame_buffer_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, res_width, res_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		//attach frame buffer texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame_buffer_texture, 0);
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Graphics Frame buffer is not complete" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//generate geometry buffer
		glGenFramebuffers(1, &g_buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);

		//position buffer
		glGenTextures(1, &position_texture);
		glBindTexture(GL_TEXTURE_2D, position_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, res_width, res_height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position_texture, 0);

		//normal buffer
		glGenTextures(1, &normals_texture);
		glBindTexture(GL_TEXTURE_2D, normals_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, res_width, res_height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normals_texture, 0);

		//albedo specular buffer
		glGenTextures(1, &albedo_specular_texture);
		glBindTexture(GL_TEXTURE_2D, albedo_specular_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, res_width, res_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedo_specular_texture, 0);
		
		//normalmap buffer
		glGenTextures(1, &normal_map_texture);
		glBindTexture(GL_TEXTURE_2D, normal_map_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, res_width, res_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, normal_map_texture, 0);

		//tangents buffer
		glGenTextures(1, &tangents_texture);
		glBindTexture(GL_TEXTURE_2D, tangents_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, res_width, res_height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, tangents_texture, 0);

		//use 3 attachments
		GLuint attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
		glDrawBuffers(5, attachments);

		//use renderbuffer object as depth and stencil buffer
		GLuint rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, res_width, res_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Graphics Geometry buffer is not complete" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void clear() {
		if (frame_quad != nullptr) delete frame_quad;
		if(primitive_quad != nullptr) delete primitive_quad;
		if(primitive_cube != nullptr) delete primitive_cube;
		frame_quad = nullptr;
		primitive_quad = nullptr;
		primitive_cube = nullptr;
	}

	void clear_buffers() {
		glDeleteFramebuffers(1, &frame_buffer);
		glDeleteFramebuffers(1, &g_buffer);
		glDeleteTextures(1, &frame_buffer_texture);
		glDeleteTextures(1, &position_texture);
		glDeleteTextures(1, &normals_texture);
		glDeleteTextures(1, &albedo_specular_texture);
		glDeleteTextures(1, &normal_map_texture);
		glDeleteTextures(1, &tangents_texture);
	}

	void render() {
		//TODO: multiple geometry buffers per camera
		glViewport(0, 0, res_width, res_height);
		for (Camera* camera : components::cameras) {
			//First pass: geometry buffer rendering
			glBindFramebuffer(GL_FRAMEBUFFER, g_buffer);
			//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			for (MeshRenderer* mr : components::meshrenderers) {
				mr->render_geometry(camera, g_buffer_shader);
			}

			//Second pass: lightning pass
			glCullFace(GL_FRONT);
			//render directionallights shadow maps if necessary
			for (int i = 0; i < components::directionallights.size(); ++i) {
				DirectionalLight* dl = components::directionallights[i];
				if (dl->get_cast_shadows()) {
					dl->use_shadow_buffer(depth_shader);
					for (MeshRenderer* mr : components::meshrenderers) {
						mr->render_depth(depth_shader);
					}
				}
			}
			glCullFace(GL_BACK);

			//use lightning shader
			glViewport(0, 0, res_width, res_height);
			glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(lightning_shader->get_id());
			glBindVertexArray(frame_quad->vao);

			//bind gbuffer
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, position_texture);
			glUniform1i(lightning_shader->get_location("g_position"), 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normals_texture);
			glUniform1i(lightning_shader->get_location("g_normal"), 1);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, albedo_specular_texture);
			glUniform1i(lightning_shader->get_location("g_albedo_specular"), 2);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, normal_map_texture);
			glUniform1i(lightning_shader->get_location("g_normal_map"), 3);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, tangents_texture);
			glUniform1i(lightning_shader->get_location("g_tangents"), 4);

			GLuint current_texture = 0;

			//bind pointlights
			glUniform1i(lightning_shader->get_location("current_pointlights"), components::pointlights.size());
			for (int i = 0; i < components::pointlights.size(); ++i) {
				std::string n = std::to_string(i);
				PointLight* pl = components::pointlights[i];
				glUniform3fv(lightning_shader->get_location("pointlights[" + n + "].position"), 1, &pl->get_position()[0]);
				glUniform3fv(lightning_shader->get_location("pointlights[" + n + "].color"), 1, &pl->get_color()[0]);
				glUniform1f(lightning_shader->get_location("pointlights[" + n + "].intensity"), pl->get_intensity());
				glUniform1f(lightning_shader->get_location("pointlights[" + n + "].constant"), pl->get_constant());
				glUniform1f(lightning_shader->get_location("pointlights[" + n + "].linear"), pl->get_linear());
				glUniform1f(lightning_shader->get_location("pointlights[" + n + "].quadratic"), pl->get_quadratic());
			}

			//bind directionallights
			glUniform1i(lightning_shader->get_location("current_directionallights"), components::directionallights.size());
			for (int i = 0; i < components::directionallights.size(); ++i) {
				std::string n = std::to_string(i);
				DirectionalLight* dl = components::directionallights[i];
				
				glUniform3fv(lightning_shader->get_location("directionallights[" + n + "].direction"), 1, &dl->get_direction()[0]);
				glUniform3fv(lightning_shader->get_location("directionallights[" + n + "].color"), 1, &dl->get_color()[0]);
				glUniform1f(lightning_shader->get_location("directionallights[" + n + "].intensity"), dl->get_intensity());

				glUniform1i(lightning_shader->get_location("directionallights[" + n + "].cast_shadows"), dl->get_cast_shadows());
				if (dl->get_cast_shadows()) {
					glm::mat4 light_space = dl->get_light_projection() * dl->get_light_view();
					glUniformMatrix4fv(lightning_shader->get_location("directionallights[" + n + "].light_space"), 1, GL_FALSE, &light_space[0][0]);
					glActiveTexture(GL_TEXTURE5 + current_texture);
					glBindTexture(GL_TEXTURE_2D, dl->get_shadow_map());
					glUniform1i(lightning_shader->get_location("directionallights[" + n + "].shadow_map"), 5 + current_texture);
				}
			}

			//bind view position
			glUniform3fv(lightning_shader->get_location("view_position"), 1, &camera->get_position()[0]);

			//draw
			glDrawElements(GL_TRIANGLES, frame_quad->indices.size(), GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);

			//unbind gbuffer
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, 0);

			//Third pass: screen rendering (post processing)
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(frame_buffer_shader->get_id());
			glBindVertexArray(frame_quad->vao);
			glDisable(GL_DEPTH_TEST);

			glActiveTexture(GL_TEXTURE0);
			glViewport(0, 0, res_width, res_height);
			glBindTexture(GL_TEXTURE_2D, frame_buffer_texture);
			glUniform1i(frame_buffer_shader->get_location("frame_texture"), 0);

			glUniform1f(frame_buffer_shader->get_location("exposure"), camera->get_exposure());

			glDrawElements(GL_TRIANGLES, frame_quad->indices.size(), GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void set_resolution(GLuint width, GLuint height) {
		res_width = width;
		res_height = height;
		//TODO: test this
		clear_buffers();
		init_buffers();
	}

	Mesh* get_primitive_quad() {
		return primitive_quad;
	}

	Mesh* get_primitive_cube() {
		return primitive_cube;
	}

	void load_frame_quad() {
		std::vector<glm::vec3> vertices;
		std::vector<GLuint> indices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;

		vertices.push_back(glm::vec3(-1.0f, -1.0f, 0));
		vertices.push_back(glm::vec3(1.0f, -1.0f, 0));
		vertices.push_back(glm::vec3(-1.0f, 1.0f, 0));
		vertices.push_back(glm::vec3(1.0f, 1.0f, 0));

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(3);

		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));

		uvs.push_back(glm::vec2(0, 0));
		uvs.push_back(glm::vec2(1, 0));
		uvs.push_back(glm::vec2(0, 1));
		uvs.push_back(glm::vec2(1, 1));

		frame_quad = new Mesh();
		frame_quad->vertices = vertices;
		frame_quad->indices = indices;
		frame_quad->normals = normals;
		frame_quad->uvs = uvs;
		frame_quad->apply();
	}

	void load_primitive_quad() {
		std::vector<glm::vec3> vertices;
		std::vector<GLuint> indices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;

		vertices.push_back(glm::vec3(-0.5f, -0.5f, 0));
		vertices.push_back(glm::vec3(0.5f, -0.5f, 0));
		vertices.push_back(glm::vec3(-0.5f, 0.5f, 0));
		vertices.push_back(glm::vec3(0.5f, 0.5f, 0));

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(1);
		indices.push_back(3);

		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));

		uvs.push_back(glm::vec2(0, 0));
		uvs.push_back(glm::vec2(1, 0));
		uvs.push_back(glm::vec2(0, 1));
		uvs.push_back(glm::vec2(1, 1));

		primitive_quad = new Mesh();
		primitive_quad->vertices = vertices;
		primitive_quad->indices = indices;
		primitive_quad->normals = normals;
		primitive_quad->uvs = uvs;

		primitive_quad->calculate_tbm();
		primitive_quad->apply();
	}

	void load_primitive_cube() {
		std::vector<glm::vec3> vertices;
		std::vector<GLuint> indices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;

		//front
		vertices.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
		vertices.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
		vertices.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));
		vertices.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		normals.push_back(glm::vec3(0, 0, 1));
		//back
		vertices.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
		vertices.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));
		vertices.push_back(glm::vec3(0.5f, -0.5f, -0.5f));
		vertices.push_back(glm::vec3(0.5f, 0.5f, -0.5f));
		normals.push_back(glm::vec3(0, 0, -1));
		normals.push_back(glm::vec3(0, 0, -1));
		normals.push_back(glm::vec3(0, 0, -1));
		normals.push_back(glm::vec3(0, 0, -1));
		//left
		vertices.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
		vertices.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
		vertices.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));
		vertices.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));
		normals.push_back(glm::vec3(-1, 0, 0));
		normals.push_back(glm::vec3(-1, 0, 0));
		normals.push_back(glm::vec3(-1, 0, 0));
		normals.push_back(glm::vec3(-1, 0, 0));
		//right
		vertices.push_back(glm::vec3(0.5f, -0.5f, -0.5f));
		vertices.push_back(glm::vec3(0.5f, 0.5f, -0.5f));
		vertices.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
		vertices.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
		normals.push_back(glm::vec3(1, 0, 0));
		normals.push_back(glm::vec3(1, 0, 0));
		normals.push_back(glm::vec3(1, 0, 0));
		normals.push_back(glm::vec3(1, 0, 0));
		//top
		vertices.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));
		vertices.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));
		vertices.push_back(glm::vec3(0.5f, 0.5f, -0.5f));
		vertices.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
		normals.push_back(glm::vec3(0, 1, 0));
		normals.push_back(glm::vec3(0, 1, 0));
		normals.push_back(glm::vec3(0, 1, 0));
		normals.push_back(glm::vec3(0, 1, 0));
		//bottom
		vertices.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
		vertices.push_back(glm::vec3(0.5f, -0.5f, -0.5f));
		vertices.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
		vertices.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
		normals.push_back(glm::vec3(0, -1, 0));
		normals.push_back(glm::vec3(0, -1, 0));
		normals.push_back(glm::vec3(0, -1, 0));
		normals.push_back(glm::vec3(0, -1, 0));

		for (int i = 0; i < 6; ++i) {
			uvs.push_back(glm::vec2(0, 0));
			uvs.push_back(glm::vec2(1, 0));
			uvs.push_back(glm::vec2(0, 1));
			uvs.push_back(glm::vec2(1, 1));

			indices.push_back(i * 4 + 0);
			indices.push_back(i * 4 + 1);
			indices.push_back(i * 4 + 2);
			indices.push_back(i * 4 + 2);
			indices.push_back(i * 4 + 1);
			indices.push_back(i * 4 + 3);
		}

		primitive_cube = new Mesh();
		primitive_cube->vertices = vertices;
		primitive_cube->indices = indices;
		primitive_cube->normals = normals;
		primitive_cube->uvs = uvs;

		primitive_cube->calculate_tbm();
		primitive_cube->apply();
	}
}