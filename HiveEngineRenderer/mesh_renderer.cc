//
// Created by calpe on 7/29/2018.
//

#include "mesh_renderer.h"

namespace HiveEngineRenderer {
    MeshRenderer::MeshRenderer() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glGenBuffers(1, &CBO);
        render_size = 0;
    }

    MeshRenderer::~MeshRenderer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &CBO);
    }

    bool MeshRenderer::init() {
        auto vertex_shader = ShaderLoader::load("GL_CODE/mesh.vs", GL_VERTEX_SHADER);
        if (vertex_shader == 0) {
            return false;
        }

        auto fragment_shader =  ShaderLoader::load("GL_CODE/mesh.fs", GL_FRAGMENT_SHADER);
        if (fragment_shader == 0) {
            return false;
        }

        try {
            this->program = ShaderProgram(vertex_shader, fragment_shader);
        }
        catch (std::exception e) {
            return false;
        }

        program.use();

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);

        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void *) 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return true;
    }

    void MeshRenderer::render(glm::mat4 view_matrix, glm::mat4 transform) {
        program.use();

        glUniformMatrix4fv(glGetUniformLocation(this->program.get_id(), "view"), 1, GL_FALSE,
                           glm::value_ptr(view_matrix));
        glUniformMatrix4fv(glGetUniformLocation(this->program.get_id(), "transform"), 1, GL_FALSE,
                           glm::value_ptr(transform));

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, render_size, GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

    void MeshRenderer::upload(HiveEngine::Mesh *mesh) {
        if (mesh->vertices.size() == 0) return;
        program.use();

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(glm::vec3), mesh->vertices.data(),
                     GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBufferData(GL_ARRAY_BUFFER, mesh->colors.size() * sizeof(glm::vec4), mesh->colors.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(glm::uvec3), mesh->indices.data(),
                     GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        render_size = mesh->indices.size() * 3;
    }


}