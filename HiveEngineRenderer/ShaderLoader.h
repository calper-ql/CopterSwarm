#ifndef __HIVEENGINE_RENDERER_SHADER_LOADER_H
#define __HIVEENGINE_RENDERER_SHADER_LOADER_H

#include "opengl_settings.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "file_loader.h"

namespace HiveEngineRenderer {
    class ShaderLoader {
    public:
        static GLuint load(std::string location, GLenum shaderType);

        static GLuint load_from_str(std::string src, GLenum shaderType);
    };


}

#endif
