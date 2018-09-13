#include "GLHandler.h"

namespace HiveEngineRenderer {
    int initGL() {
        /* Initialize the library */
        if (glfwInit() == 0) {
            std::cout << "Failed to initialize GLFW" << std::endl;
            return 1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        return 0;
    }

    int initGLAD() {
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return 2;
        }
        return 0;
    }

}
