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

    auto e = new HiveEngine::Entity(glm::vec3(0.0, 0.0, 2.0), 0.1, 200.0);
    auto e2 = new HiveEngine::Entity(glm::vec3(0.0, 0.0, 0.8), 0.1, 10.0);
    auto e4 = new HiveEngine::Entity(glm::vec3(-0.8, 0.0, 0.0), 0.1, 1.0);
    e->add_child(e2);
    e2->add_child(e4);
    e2->set_torque_resistance({0, 0, 0}); // I know its weird but it works...

    e->apply_force(e->get_position() + glm::vec3(0.0, 0.0, e->get_radius()), glm::vec3(0.0, 8, 0.0), true);
    e->apply_force(e->get_position() + glm::vec3(0.0, 0.0, -e->get_radius()), glm::vec3(0.0, -8, 0.0), true);

    e2->apply_force(glm::vec3(0.0, 0.0, 0.0), glm::vec3(5, 0, 0), true);
    e->apply_force(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-5, 0, 0), true);

    std::vector<HiveEngine::Entity*> fragments;
    bool fragment_key_state = false;

    fragments.push_back(e);
    fragments.push_back(e2);
    fragments.push_back(e4);

    while (!glfwWindowShouldClose(window)) {

        //e4->apply_force(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-0.01, 0.0, 0.0), false);

        camera.set_perspective(90, camera_perspective_ratio, 0.01, 1e5);
        auto view = camera.get_view();
        camera.get_user_input(window, false);

        view = glm::translate(view, -e->get_position());
        if(e->get_position().x < -80) e->set_position(glm::vec3(80.0, 0.0, 0.3));

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

        auto e_out = e->step(30);

        std::vector<glm::vec3> local_lines;
        std::vector<glm::vec3> local_line_colors;

        glm::vec3 relative_point(0.0, 0.0, 0.1);
        auto throw_acc = e2->calculate_throw_vector(relative_point, true) + e2->get_velocity() * 2.0;
        auto global_point = e2->calculate_position() + e2->calculate_rotation_matrix() * relative_point;
        local_lines.emplace_back(global_point);
        local_lines.emplace_back(global_point +  throw_acc);
        local_line_colors.emplace_back(1.0, 1.0, 1.0);
        local_line_colors.emplace_back(1.0, 0.0, 0.0);
        ld->draw(local_lines.data(), local_line_colors.data(), local_lines.size() / 2, view);

        char_d = consolas.render("tot acc: " + std::to_string(glm::length(throw_acc)),
                                 -1.0f, char_d,
                                 text_scale,
                                 text_scale*camera_perspective_ratio);

        if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){
            if(!fragment_key_state){
                fragment_key_state = true;
                auto frag = new HiveEngine::Entity(global_point, 0.01, 0.01);
                frag->set_velocity(throw_acc);
                fragments.push_back(frag);
            }
        } else {
            fragment_key_state = false;
        }

        if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
            e2->set_position(glm::vec3(0.0, 0.0, 0.8));
        }

        if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
            e2->set_position(glm::vec3(0.0, 0.0, 0.4));
        }

        if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
            //e3->set_position(glm::vec3(0.0, 0.0, -0.8));
        }

        if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
            //e3->set_position(glm::vec3(0.0, 0.0, -0.4));
        }

        for (const auto &fragment : fragments) {
            fragment->step(30);
            auto line_pair = HiveEngine::generate_entity_line_description(fragment, glm::vec3(1.0, 1.0, 1.0));
            ld->draw(line_pair.first.data(), line_pair.second.data(), line_pair.first.size() / 2, view);
        }

        auto central_mass = e->calculate_central_mass();
        auto mass_center = e->calculate_position() + central_mass.position;
        std::cout << HiveEngine::vec3_to_str(mass_center) << std::endl;
        auto line_pair = HiveEngine::generate_target_line_description(mass_center, 0.1, glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 0.0, 0.0));
        ld->draw(line_pair.first.data(), line_pair.second.data(), line_pair.first.size() / 2, view);


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
