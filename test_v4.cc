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
#include "HiveEngine/Utilities.h"
#include "CopterLib/Environment.h"

float camera_perspective_ratio = 1.0;
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#define PI 3.14159265359

int main(int argc, char* argv[]){
    std::cout << "-- START --" << std::endl;

    if (HiveEngineRenderer::initGL()) {
        std::cout << "initGL error" << std::endl;
        return 3;
    }

    GLFWwindow *window = glfwCreateWindow(1000, 1000, "test_v4", NULL, NULL);
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

    CopterLib::Copter* c = new CopterLib::Copter(glm::vec3(0.0, 0.0, 10.0), 1.0, 0.2);
    CopterLib::Motor* mlu = new CopterLib::Motor(300, 0.05, glm::vec3(-1.0, 1.0, 0.0), 0.1, 0.1);
    CopterLib::Motor* mll = new CopterLib::Motor(300, 0.05, glm::vec3(-1.0, -1.0, 0.0), 0.1, 0.1);
    CopterLib::Motor* mru = new CopterLib::Motor(300, 0.05, glm::vec3(1.0, 1.0, 0.0), 0.1, 0.1);
    CopterLib::Motor* mrl = new CopterLib::Motor(300, 0.05, glm::vec3(1.0, -1.0, 0.0), 0.1, 0.1);

	/* 		Energy stored in battery is Charge(C) * Voltage(V) = Energy(J)
	 *		Also Current(A) * time(seconds) = Charge(C)
	 *
	 *		mA means 1/1000 Amp for an hour and hour has a 60x60 = 3600 seconds
	 *
	 *		1 mAh = 0.001 Amps * 3600 seconds = 3.6 Coulombs of charge
	 *
	 *		A 12 Volt battery with 3600 mAh has total of 3600*12*3.6 = Joules of energy ()
	 * 
	 */
    CopterLib::EnergySource* es = new CopterLib::EnergySource(3600.0f*12.0f*3.6f, 3600.0f*12.0f*3.6f, glm::vec3(0.0, 0.0, 0.1), 0.1, 0.1);

    c->set_energy_source(es);

    CopterLib::Rotor* rlu = new CopterLib::Rotor(4.0f, 0.09f * 1.225f * 1.0f, true, glm::vec3(0.0, 0.0, 0.1), 0.2, 0.1);
    CopterLib::Rotor* rll = new CopterLib::Rotor(4.0f, 0.09f * 1.225f * 1.0f, true, glm::vec3(0.0, 0.0, 0.1), 0.2, 0.1);
    CopterLib::Rotor* rru = new CopterLib::Rotor(4.0f, 0.09f * 1.225f * 1.0f, true, glm::vec3(0.0, 0.0, 0.1), 0.2, 0.1);
    CopterLib::Rotor* rrl = new CopterLib::Rotor(4.0f, 0.09f * 1.225f * 1.0f, true, glm::vec3(0.0, 0.0, 0.1), 0.2, 0.1);

    c->add_actuator({mlu, rlu});
    c->add_actuator({mll, rll});
    c->add_actuator({mrl, rrl});
    c->add_actuator({mru, rru});

    rlu->set_torque_resistance(glm::vec3(1.0, 1.0, 0.0));
    rll->set_torque_resistance(glm::vec3(1.0, 1.0, 0.0));
    rrl->set_torque_resistance(glm::vec3(1.0, 1.0, 0.0));
    rru->set_torque_resistance(glm::vec3(1.0, 1.0, 0.0));

    std::vector<HiveEngine::Entity*> fragments;

    fragments.push_back(c);
    fragments.push_back(mlu);
    fragments.push_back(mll);
    fragments.push_back(mru);
    fragments.push_back(mrl);
    fragments.push_back(rlu);
    fragments.push_back(rll);
    fragments.push_back(rru);
    fragments.push_back(rrl);
    fragments.push_back(es);

    //c->set_throttle({-0.0, -0.0, 0.01, 0.0});

    CopterLib::Environment env;
    env.command(c->serialize());
    env.command(es->serialize());

    env.command(mlu->serialize());
    env.command(mll->serialize());
    env.command(mru->serialize());
    env.command(mrl->serialize());

    env.command(rlu->serialize());
    env.command(rll->serialize());
    env.command(rru->serialize());
    env.command(rrl->serialize());


    while (!glfwWindowShouldClose(window)) {
        float step_val = 100;
        camera.set_perspective(90, camera_perspective_ratio, 0.01, 1e5);
        auto view = camera.get_view();
        camera.get_user_input(window, false);

        //view = glm::translate(view, -c->get_position());
        if(c->calculate_position().z < 0.0) {
            c->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
            c->set_velocity(glm::vec3(0.0f, 0.0f, 0.0f));
            for (const auto &fragment : fragments) {
                fragment->apply_force(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 9.8f/step_val * fragment->get_mass()), false);
            }
        }

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
        char_d = consolas.render("BATT: " + std::to_string(es->get_available_energy()) + "/" +std::to_string(es->get_capacity()),
                                 -1.0f, char_d,
                                 text_scale,
                                 text_scale*camera_perspective_ratio);


        for (const auto &fragment : fragments) {
            fragment->apply_force(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -9.8f/step_val * fragment->get_mass()), false);
        }

        float fov_mod = 0.0;
        float bck_mod = 0.0;
		float left_mod = 0.0;
		float right_mod = 0.0;
        float pow = 0.0;

        if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
            pow = 0.99;
        }
        if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
            fov_mod = 0.03;
        }

        if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
            bck_mod = 0.03;
        }
		
	  	if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
            left_mod = 0.03;
        }

        if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
            right_mod = 0.03;
        }


        c->set_throttle({-pow+fov_mod, pow+left_mod, -pow+bck_mod, pow+right_mod});

        for (auto fragment : fragments) {
            auto line_pair = HiveEngine::generate_entity_line_description(fragment, glm::vec3(1.0, 1.0, 0.1));
            ld->draw(line_pair.first.data(), line_pair.second.data(), line_pair.first.size() / 2, view);
        }

        std::vector<glm::vec3> local_lines;
        std::vector<glm::vec3> local_line_colors;
        glm::vec3 relative_point(0.0, 0.1, 0.0);
        auto throw_acc = rrl->calculate_throw_vector(relative_point, true)/step_val + rrl->get_velocity() * 2.0 / 60.0;
        auto global_point = rrl->calculate_position() + rrl->calculate_rotation_matrix() * relative_point;
        local_lines.emplace_back(global_point);
        local_lines.emplace_back(global_point +  throw_acc);
        local_line_colors.emplace_back(1.0, 1.0, 1.0);
        local_line_colors.emplace_back(1.0, 0.0, 0.0);
        ld->draw(local_lines.data(), local_line_colors.data(), local_lines.size() / 2, view);

        auto central_mass = c->calculate_central_mass();
        auto mass_center = c->calculate_position() + central_mass.position;
        std::cout << HiveEngine::vec3_to_str(mass_center) << std::endl;
        auto line_pair = HiveEngine::generate_target_line_description(mass_center, 0.1, glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 0.0, 0.0));
        ld->draw(line_pair.first.data(), line_pair.second.data(), line_pair.first.size() / 2, view);

        c->step(step_val);

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
