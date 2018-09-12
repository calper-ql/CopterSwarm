//
// Created by calpe on 8/31/2018.
//

#include "TextRenderer.h"


namespace HiveEngineRenderer {
    BitmapImage readBMP(const std::string &file) {
        static constexpr size_t HEADER_SIZE = 54;
        BitmapImage bi;

        std::ifstream bmp(file, std::ios::binary);

        std::array<char, HEADER_SIZE> header;
        bmp.read(header.data(), header.size());

        auto fileSize = *reinterpret_cast<uint32_t *>(&header[2]);
        auto dataOffset = *reinterpret_cast<uint32_t *>(&header[10]);
        auto width = *reinterpret_cast<uint32_t *>(&header[18]);
        auto height = *reinterpret_cast<uint32_t *>(&header[22]);
        auto depth = *reinterpret_cast<uint16_t *>(&header[28]);

        //std::cout << "fileSize: " << fileSize << std::endl;
        //std::cout << "dataOffset: " << dataOffset << std::endl;
        //std::cout << "width: " << width << std::endl;
        //std::cout << "height: " << height << std::endl;
        //std::cout << "depth: " << depth << "-bit" << std::endl;

        std::vector<char> img(dataOffset - HEADER_SIZE);
        bmp.read(img.data(), img.size());

        auto dataSize = ((width * 3 + 3) & (~3)) * height;
        img.resize(dataSize);
        bmp.read(img.data(), img.size());

        //std::cout << "vec size: " << img.size() << std::endl;

        bi.height = height;
        bi.width = width;
        bi.depth = depth;
        bi.data_offset = dataOffset;
        bi.file_size = fileSize;
        bi.data = img;

        return bi;
    }

    TextRenderer::TextRenderer(std::string name) {
        this->inited = false;
        this->name = name;
        char_data = std::vector<TextChar *>(256, nullptr);
    }

    int __text_renderer_str_to_int(std::string str) {
        std::stringstream ss(str);
        int num;
        ss >> num;
        return num;
    }

    bool TextRenderer::init() {
        image = readBMP(name + ".bmp");
        if (image.width == 0)
            return false;

        std::ifstream file(name + ".csv");
        if (file.fail())
            return false;

        std::map<std::string, int> csv_dict;
        std::vector<std::string> csv_vals;

        std::string value;
        int counter = 0;
        while (file.good()) {
            std::getline(file, value);
            if (value.length() > 0) {
                std::string first = value.substr(0U, value.find(','));
                std::string second = value.substr(value.find(',') + 1, value.length());
                //std::cout << first << " ---> " << second << std::endl;
                csv_dict[first] = counter;
                counter++;
                csv_vals.push_back(second);
            }
        }

        //std::cout << "-----START------" << std::endl;
        for (auto key: csv_dict) {
            //std::cout << "Key: " << key.first << " Val: " << key.second << std::endl;
        }
        //std::cout << "------END--------" << std::endl;

        int cell_width = __text_renderer_str_to_int(csv_vals[2]);
        int cell_height = __text_renderer_str_to_int(csv_vals[3]);
        //std::cout << "cell width: " << cell_width << std::endl;
        //std::cout << "cell height: " << cell_height << std::endl;

        int font_height = __text_renderer_str_to_int(csv_vals[6]);
        //std::cout << "font height: " << font_height << std::endl;

        int h_cell_count = image.width / cell_width;
        //std::cout << "h_cell count: " << h_cell_count << std::endl;

        int v_cell_count = image.height / cell_height;
        //std::cout << "v_cell count: " << v_cell_count << std::endl;

        int start_char = __text_renderer_str_to_int(csv_vals[4]);
        //std::cout << "start char: " << start_char << std::endl;

        int current = start_char;
        for (int j = 0; j < v_cell_count; j++) {
            for (int i = 0; i < h_cell_count; i++) {
                auto new_char = new TextChar();
                new_char->x = (i * cell_width) / (float) image.width;
                new_char->y = 1.0 - ((j * cell_height) / (float) image.height) - ((font_height) / (float) image.height);
                new_char->width = 9 / (float) image.height;
                new_char->height = font_height / (float) image.width;
                char_data[current] = new_char;
                current++;
            }
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     image.data.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        auto vertex_shader = ShaderLoader::load("GL_CODE/text.vs", GL_VERTEX_SHADER);
        if (vertex_shader == 0) {
            return false;
        }

        auto fragment_shader = ShaderLoader::load("GL_CODE/text.fs", GL_FRAGMENT_SHADER);
        if (fragment_shader == 0) {
            return false;
        }

        try {
            this->program = ShaderProgram(vertex_shader, fragment_shader);
        }
        catch (std::exception &e) {
            return false;
        }

        inited = true;
        return true;
    }


    float TextRenderer::render(std::string str, float x, float y, float x_scale, float y_scale) {
        float y_min = 0.0;
        if (inited) {
            auto xt = x;
            for (auto c: str) {
                auto ch = this->char_data[c];
                if (y_min < ch->height) y_min = ch->height;
                render(c, xt, y, x_scale, y_scale);
                xt += ch->width * x_scale;
            }
        }
        return y - y_min * y_scale;
    }

    void TextRenderer::render(char ch, float x, float y, float x_scale, float y_scale) {
        if (inited) {
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            auto c = this->char_data[ch];
            auto xw = c->width * x_scale;
            auto yw = c->height * y_scale;

            float vertices[] = {
                    // positions                        // colors          // texture coords
                    x + xw, y, 0.0f, 1.0f, 0.0f, 0.0f, c->x + c->width, c->y + c->height,   // top right
                    x + xw, y - yw, 0.0f, 0.0f, 1.0f, 0.0f, c->x + c->width, c->y,               // bottom right
                    x, y - yw, 0.0f, 0.0f, 0.0f, 1.0f, c->x, c->y,               // bottom left
                    x, y, 0.0f, 1.0f, 1.0f, 0.0f, c->x, c->y + c->height    // top left
            };

            unsigned int indices[] = {
                    0, 1, 3, // first triangle
                    1, 2, 3  // second triangle
            };

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
            glEnableVertexAttribArray(0);
            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            // texture coord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            program.use();
            glBindTexture(GL_TEXTURE_2D, texture);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }


}


/*

unsigned char* readBMP(char* filename)
{
    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);

    for(i = 0; i < size; i += 3)
    {
            unsigned char tmp = data[i];
            data[i] = data[i+2];
            data[i+2] = tmp;
    }

    return data;
}

 Now data should contain the (R, G, B) values of the pixels. The color of pixel (i, j) is stored at
 data[3 * (i * width + j)], data[3 * (i * width + j) + 1] and data[3 * (i * width + j) + 2].

In the last part, the swap between every first and third pixel is done because
windows stores the color values as (B, G, R) triples, not (R, G, B).

 */