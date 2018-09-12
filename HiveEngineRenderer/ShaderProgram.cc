#include "ShaderProgram.h"

namespace HiveEngineRenderer {
    ShaderProgram::ShaderProgram() {

    }

    ShaderProgram::ShaderProgram(std::string vertex_loc, std::string fragment_loc) {
        GLuint vertex = ShaderLoader::load(vertex_loc, GL_VERTEX_SHADER);
        if (vertex == 0) throw std::exception();
        GLuint fragment = ShaderLoader::load(fragment_loc, GL_FRAGMENT_SHADER);
        if (fragment == 0) throw std::exception();

        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        int success;
        char infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cout << infoLog << std::endl;
            throw std::exception();
        }
    }

    ShaderProgram::ShaderProgram(GLuint vertex, GLuint fragment) {
        if (vertex == 0) throw std::exception();
        if (fragment == 0) throw std::exception();

        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        int success;
        char infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cout << infoLog << std::endl;
            throw std::exception();
        }
    }


    void ShaderProgram::use() {
        glUseProgram(program);
    }

    GLuint ShaderProgram::get_id() {
        return program;
    }
}
