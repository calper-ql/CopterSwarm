/* Written by Can Alper
 *
 * Simple c++ program to test visible engine behaviour.
 *
 */

#include <iostream>

#include "HiveEngineRenderer/GLHandler.h"
#include "HiveEngineRenderer/TextRenderer.h"
#include "HiveEngineRenderer/Camera.h"
#include "HiveEngineRenderer/LineRenderer.h"

#include "CopterLib/Copter.h"

float camera_perspective_ratio = 1.0;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#define PI 3.14159265359

int main(int argc, char* argv[]){
    std::cout << "-- START --" << std::endl;

    if (HiveEngineRenderer::initGL()) {
        std::cout << "initGL error" << std::endl;
        return 3;
    }

    GLFWwindow *window = glfwCreateWindow(1000, 1000, "test_v2", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (HiveEngineRenderer::initGLAD()) {
        std::cout << "initGLAD error" << std::endl;
        return 4;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetWindowSizeCallback(window, framebuffer_size_callback);

    // Camera code
    glm::vec3 position = {0, 0.0, 10};
    auto camera = HiveEngineRenderer::Camera();
    camera.set_perspective(90, camera_perspective_ratio, 1e3, 1e10);
    camera.set_position(position);

    std::vector<glm::vec3> lines;
    std::vector<glm::vec3> line_colors;

    for (float i = -100; i < 100; i += 1.0f) {
        // X axis aligned line
        lines.emplace_back(i, -100, 0);
        lines.emplace_back(i, 100, 0);
        line_colors.emplace_back(0, 0, 1);
        line_colors.emplace_back(0, 0, 1);

        // Y axis aligned line
        lines.emplace_back(-100, i, 0);
        lines.emplace_back(100, i, 0);
        line_colors.emplace_back(0, 0, 1);
        line_colors.emplace_back(0, 0, 1);
    }

    auto ld = new HiveEngineRenderer::LineRenderer();
    ld->init();

    HiveEngineRenderer::TextRenderer consolas("GL_CODE/consolas");
    consolas.init();

    /* // Real choppa
    CopterLib::Copter* c = new CopterLib::Copter(glm::vec3(0.0, 0.0, 1.0), 0.1, 1.0);
    c->add_actuator({new CopterLib::Motor(300, glm::vec3(0.0, 0.0, 0.0), 0.05, 0.05),
                     new CopterLib::Rotor(4.0f, 0.09f * 1.225f * 1.0f, false, glm::vec3(0.0, 0.0, 0.01), 0.5, 0.2)});

    auto rear_rotor = new CopterLib::Rotor(4.0f, 0.09f * 1.225f * 1.0f, false, glm::vec3(0.0, 0.7, 0.01), 0.05, 0.2);
    c->add_actuator({new CopterLib::Motor(30, glm::vec3(0.0, 0.0, 0.0), 0.01, 0.01), rear_rotor});
    rear_rotor->set_rotation_matrix(HiveEngine::generate_rotation_matrix('z', PI/2.0));
    */
    CopterLib::Copter* c = new CopterLib::Copter(glm::vec3(0.0, 0.0, 1.0), 0.1, 1.0);



    while (!glfwWindowShouldClose(window)) {

        camera.set_perspective(90, camera_perspective_ratio, 0.01, 1e5);
        auto view = camera.get_view();
        camera.get_user_input(window, false);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ld->draw(lines.data(), line_colors.data(), lines.size() / 2, view);

        float char_d;
        float text_scale = 0.7f;
        char_d = consolas.render("traverse_modifier: " + std::to_string(camera.traverse_modifier),
                                 -1.0f, 1.0f,
                                 text_scale,
                                 text_scale*camera_perspective_ratio);
        char_d = consolas.render("rotation_modifier: " + std::to_string(camera.rotate_modifier),
                                 -1.0f, char_d,
                                 text_scale,
                                 text_scale*camera_perspective_ratio);



        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "--  END  --" << std::endl;
    return 0;
}



void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    (void) window;
#ifdef __APPLE__
    glViewport(0, 0, width * 2, height * 2);
#else
    glViewport(0, 0, width, height);
#endif
    camera_perspective_ratio = width / (float) height;
}
