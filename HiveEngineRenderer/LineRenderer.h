#ifndef __HIVEENGINE_RENDERER_LINE_DRAWER_H
#define __HIVEENGINE_RENDERER_LINE_DRAWER_H

#include "opengl_settings.h"
#include "ShaderProgram.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/ext.hpp"

namespace HiveEngineRenderer {
    class LineRenderer {
    private:
        GLuint VBO, colorVBO, VAO;
        GLuint tex_id;
        ShaderProgram program;
    public:
        LineRenderer();

        ~LineRenderer();

        bool init();

        void draw(glm::vec3 *lines, glm::vec3 *colors, size_t size, glm::mat4 view_matrix);

        GLuint getProgramID();
    };
}

#endif
