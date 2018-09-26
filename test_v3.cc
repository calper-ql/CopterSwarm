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

    GLFWwindow *window = glfwCreateWindow(1000, 1000, "test_v3", NULL, NULL);
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

    auto e = new HiveEngine::Entity(glm::vec3(0.0, 0.0, 0.0), 0.2, 0.1);
    auto el = new HiveEngine::Entity(glm::vec3(-0.8, 0.0, 0.0), 0.01, 0.1);
    auto el2 = new HiveEngine::Entity(glm::vec3(-0.8, 0.0, 0.0), 0.01, 0.1);
    auto er = new HiveEngine::Entity(glm::vec3(0.8, 0.0, 0.0), 0.05, 0.1);
    auto ef = new HiveEngine::Entity(glm::vec3(0.0, 0.8, 0.0), 0.01, 0.1);
    auto eb = new HiveEngine::Entity(glm::vec3(0.0, -0.8, 0.0), 0.05, 0.1);
    e->add_child(el);
    e->add_child(er);
    //el->add_child(el2);
    e->add_child(ef);
    e->add_child(eb);
    //e2->set_torque_resistance({0, 0, 0}); // I know its weird but it works...

    std::vector<HiveEngine::Entity*> fragments;
    bool fragment_key_state = false;

    fragments.push_back(e);
    fragments.push_back(el);
    fragments.push_back(er);
    fragments.push_back(el2);
    fragments.push_back(eb);
    fragments.push_back(ef);

    while (!glfwWindowShouldClose(window)) {
        float step_val = 600;

        camera.set_perspective(90, camera_perspective_ratio, 0.01, 1e5);
        auto view = camera.get_view();
        camera.get_user_input(window, false);

        //view = view / glm::mat4(e->calculate_rotation_matrix());
        view = glm::translate(view, -e->get_position());

        if(e->calculate_position().z < 0.0) {
            e->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
            e->set_velocity(glm::vec3(0.0f, 0.0f, 0.0f));
            for (const auto &fragment : fragments) {
                fragment->apply_force(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 9.8f/step_val * fragment->get_mass()), false);

            }
            //e->apply_force(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 9.8f/step_val * e->calculate_total_mass()), false);
        }


        for (const auto &fragment : fragments) {
            fragment->apply_force(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -9.8f/step_val * fragment->get_mass()), false);
        }
        //e->apply_force(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -9.8f/step_val * e->calculate_total_mass()), false);


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

        if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
            el->apply_force({0,0,0}, {0.0, 0.0, 10.0/step_val}, true);
        }

        if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
            er->apply_force({0,0,0}, {0.0, 0.0, 10.0/step_val}, true);
        }

        if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
            ef->apply_force({0,0,0}, {0.0, 0.0, 10.0/step_val}, true);
        }

        if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
            eb->apply_force({0,0,0}, {0.0, 0.0, 10.0/step_val}, true);
        }


        if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){
            el2->apply_force({0,0,0}, {0.0, 0.0, 10.0/step_val}, true);
        }

        for (const auto &fragment : fragments) {
            auto line_pair = HiveEngine::generate_entity_line_description(fragment, glm::vec3(1.0, 1.0, 1.0));
            ld->draw(line_pair.first.data(), line_pair.second.data(), line_pair.first.size() / 2, view);
        }

        std::vector<glm::vec3> local_lines;
        std::vector<glm::vec3> local_line_colors;

        for (auto fragment : fragments) {
            auto all_f = fragment->get_applied_forces();
            for (auto f : all_f) {
                auto p = fragment->calculate_position();
                auto lev = f.leverage;
                auto vec = f.force * step_val/10.0;
                if (f.is_relative){
                    auto rot = fragment->calculate_rotation_matrix();
                    lev = rot * lev;
                    vec = rot * vec;
                }
                local_lines.emplace_back(p + lev);
                local_lines.emplace_back(p + lev + vec);
                local_line_colors.emplace_back(1.0, 1.0, 1.0);
                local_line_colors.emplace_back(1.0, 0.0, 0.0);
            }
        }

        ld->draw(local_lines.data(), local_line_colors.data(), local_lines.size() / 2, view);

        auto central_mass = e->calculate_central_mass();
        auto mass_center = e->calculate_position() + central_mass.position;
        std::cout << HiveEngine::vec3_to_str(mass_center) << std::endl;
        auto line_pair = HiveEngine::generate_target_line_description(mass_center, 0.1, glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 0.0, 0.0));
        ld->draw(line_pair.first.data(), line_pair.second.data(), line_pair.first.size() / 2, view);

        auto e_out = e->step(step_val);

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
