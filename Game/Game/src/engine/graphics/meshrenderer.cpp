#include "engine/graphics/meshrenderer.hpp"

MeshRenderer::MeshRenderer(Transform* transform) {
	this->transform = transform;
}

void MeshRenderer::render_geometry(Camera* camera, Shader* shader) {
	//use shader program
	glUseProgram(shader->get_id());

	//bind vao
	glBindVertexArray(mesh->vao);

	//mvp
	glUniformMatrix4fv(shader->get_location("model"), 1, GL_FALSE, &transform->get_local_to_world()[0][0]);
	glUniformMatrix4fv(shader->get_location("projection"), 1, GL_FALSE, &camera->get_proj_matrix()[0][0]);
	glUniformMatrix4fv(shader->get_location("view"), 1, GL_FALSE, &camera->get_view_matrix()[0][0]);

	//bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material->albedo->get_id());
	glUniform1i(shader->get_location("albedo"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, material->specular->get_id());
	glUniform1i(shader->get_location("specular"), 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, material->normal->get_id());
	glUniform1i(shader->get_location("normal_map"), 2);

	//draw the mesh
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);

	//unbind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);

	//unbind vao and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void MeshRenderer::render_depth(Shader* shader) {
	//bind vao
	glBindVertexArray(mesh->vao);

	//mvp
	glUniformMatrix4fv(shader->get_location("model"), 1, GL_FALSE, &transform->get_local_to_world()[0][0]);
	
	//draw the mesh
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);

	//unbind vao and shader program
	glBindVertexArray(0);
}