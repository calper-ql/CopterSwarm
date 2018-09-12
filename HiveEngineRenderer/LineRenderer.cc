#include "LineRenderer.h"

namespace HiveEngineRenderer {
    LineRenderer::LineRenderer() {
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &colorVBO);
        glGenVertexArrays(1, &VAO);
    }

    LineRenderer::~LineRenderer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &colorVBO);
    }

    bool LineRenderer::init() {

        auto vertex_shader = ShaderLoader::load("GL_CODE/line.vs", GL_VERTEX_SHADER);
        if (vertex_shader == 0) {
            return false;
        }

        auto fragment_shader = ShaderLoader::load("GL_CODE/line.fs", GL_FRAGMENT_SHADER);
        if (fragment_shader == 0) {
            return false;
        }

        try {
            this->program = ShaderProgram(vertex_shader, fragment_shader);
        } catch (std::exception &e) {
            return false;
        }

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        return true;
    }

    void LineRenderer::draw(glm::vec3 *lines, glm::vec3 *colors, size_t size, glm::mat4 view_matrix) {
        //BUFFERS
        program.use();

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size * 2 * sizeof(glm::vec3), lines, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
        glBufferData(GL_ARRAY_BUFFER, size * 2 * sizeof(glm::vec3), colors, GL_DYNAMIC_DRAW);

        glUniformMatrix4fv(glGetUniformLocation(this->getProgramID(), "view"), 1, GL_FALSE,
                           glm::value_ptr(view_matrix));

        glDrawArraysInstanced(GL_LINES, 0, size * 2, 1);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GLuint LineRenderer::getProgramID() {
        return program.get_id();
    }
}
