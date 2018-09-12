#ifndef __HIVEENGINE_RENDERER_SHADER_PROGRAM_H
#define __HIVEENGINE_RENDERER_SHADER_PROGRAM_H

#include <string>
#include "ShaderLoader.h"

namespace HiveEngineRenderer {
    class ShaderProgram {
    private:
        GLuint program;
    public:
        ShaderProgram();

        ShaderProgram(std::string vertex_loc, std::string fragment_loc);

        ShaderProgram(GLuint vertex, GLuint fragment);

        void use();

        GLuint get_id();
    };
}

#endif 
