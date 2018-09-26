//
// Created by calpe on 7/29/2018.
//

#ifndef HIVEENGINE_MESH_RENDERER_H
#define HIVEENGINE_MESH_RENDERER_H

#include "opengl_settings.h"
#include "../HiveEngine/mesh.h"
#include "ShaderProgram.h"
#include "../glm/gtc/type_ptr.hpp"

namespace HiveEngineRenderer {

    class MeshRenderer {
    public:
        GLuint VBO, EBO, CBO, VAO;
        ShaderProgram program;
        size_t render_size;

        MeshRenderer();

        ~MeshRenderer();

        bool init();

        void upload(HiveEngine::Mesh *mesh);

        void render(glm::mat4 view_matrix, glm::mat4 transform);

    };

}


#endif //HIVEENGINE_MESH_RENDERER_H
