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

#include "HiveEngine/Entity.h"
#include "HiveEngine/Utilities.h"

float camera_perspective_ratio = 1.0;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

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

    HiveEngine::Entity e(glm::vec3(6.0, 0.0, 0.3), 0.5, 100.0);
    HiveEngine::Entity e2(glm::vec3(0.0, 0.0, 0.4), 0.1, 10.0);
    e.add_child(&e2);

    e.apply_force(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-0.3, 0.0, 0.0), true);

    e.apply_force(glm::vec3(0.0, 0.0, e.get_radius()), glm::vec3(0.0, 0.02, 0.0), true);
    e.apply_force(glm::vec3(0.0, 0.0, -e.get_radius()), glm::vec3(0.0, -0.02, 0.0), true);

    e2.apply_force(glm::vec3(0.0, 0.0, e2.get_radius()), glm::vec3(0.0, 0.001, 0.0), true);
    e2.apply_force(glm::vec3(0.0, 0.0, -e2.get_radius()), glm::vec3(0.0, -0.001, 0.0), true);
    e2.apply_force(glm::vec3(0.0, 0.0, e2.get_radius()), glm::vec3(0.001, 0.0, 0.0), true);
    e2.apply_force(glm::vec3(0.0, 0.0, -e2.get_radius()), glm::vec3(-0.001, 0.0, 0.0), true);

    while (!glfwWindowShouldClose(window)) {


        camera.set_perspective(90, camera_perspective_ratio, 0.01, 1e5);
        auto view = camera.get_view();
        camera.get_user_input(window, false);

        view = glm::translate(view, -e.get_position());
        if(e.get_position().x < -80) e.set_position(glm::vec3(80.0, 0.0, 0.3));

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

        glLineWidth(4);
        auto line_pair = HiveEngine::generate_entity_line_description(&e, glm::vec3(1.0, 1.0, 0.3));
        ld->draw(line_pair.first.data(), line_pair.second.data(), line_pair.first.size() / 2, view);
        glLineWidth(1);

        glLineWidth(2);
        line_pair = HiveEngine::generate_entity_line_description(&e2, glm::vec3(1.0, 1.0, 1.0));
        ld->draw(line_pair.first.data(), line_pair.second.data(), line_pair.first.size() / 2, view);
        glLineWidth(1);

        e.step();

        std::vector<glm::vec3> local_lines;
        std::vector<glm::vec3> local_line_colors;

        glm::vec3 relative_point(0.0, 0.0, 0.1);
        auto throw_acc = e2.calculate_throw_acceleration(relative_point, true);
        auto global_point = e2.calculate_position() + e2.calculate_rotation_matrix() * relative_point;
        local_lines.emplace_back(global_point);
        local_lines.emplace_back(global_point +  throw_acc * 1000.0);
        local_line_colors.emplace_back(1.0, 1.0, 1.0);
        local_line_colors.emplace_back(1.0, 0.0, 0.0);
        ld->draw(local_lines.data(), local_line_colors.data(), local_lines.size() / 2, view);

        char_d = consolas.render("tot acc: " + std::to_string(glm::length(throw_acc)),
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
