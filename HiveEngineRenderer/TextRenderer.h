//
// Created by calpe on 8/31/2018.
//

#ifndef HIVEENGINE_RENDERER_TEXTRENDERER_H
#define HIVEENGINE_RENDERER_TEXTRENDERER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include <string>
#include <map>
#include "opengl_settings.h"
#include "ShaderProgram.h"
#include <sstream>

namespace HiveEngineRenderer {
    struct BitmapImage {
        unsigned width = 0;
        unsigned height = 0;
        unsigned depth = 0;
        unsigned data_offset = 0;
        unsigned file_size = 0;
        std::vector<char> data;
    };

    struct TextChar {
        float x, y;
        float width, height;
    };

    class TextRenderer {
    private:
        GLuint texture;
        GLuint VBO, VAO, EBO;
        ShaderProgram program;
        BitmapImage image;

        bool inited;
        std::string name;
        std::vector<TextChar *> char_data;

        void render(char ch, float x, float y, float x_scale, float y_scale);

    public:
        TextRenderer(std::string name);

        bool init();

        float render(std::string str, float x, float y, float x_scale, float y_scale);
    };
}


#endif //DARKENGINE_TEXTRENDERER_H
