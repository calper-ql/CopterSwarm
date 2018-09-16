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

#include "HiveEngine/Entity_Deprecated.h"
#include "HiveEngine/Utilities.h"

float camera_perspective_ratio = 1.0;
void framebuffer_size_callback(GLFWwindow *window, int width, int height); // This function changes the camera_perspective_ratio on call, it is called by glfw

int main(int argc, char* argv[]){
    std::cout << "-- START --" << std::endl;

    // This is how you initialize OpenGL, You should be able to copy paste this and change the window name
    if (HiveEngineRenderer::initGL()) {
        std::cout << "initGL error" << std::endl;
        return 3;
    }

    GLFWwindow *window = glfwCreateWindow(1000, 1000, "test_v2", NULL, NULL); // change window name here
    glfwMakeContextCurrent(window);

    if (HiveEngineRenderer::initGLAD()) {
        std::cout << "initGLAD error" << std::endl;
        return 4;
    }

    glEnable(GL_DEPTH_TEST); // this line enables depth testing , which means that pixels are rendered depending on their closeness to the camera
    // disabling depth test will make the screen render what was last rendered, which means it wont look 3d...
    glfwSetWindowSizeCallback(window, framebuffer_size_callback); // registers the callback function

    // Camera code
    glm::vec3 position = {0, 0.0, 10};
    auto camera = HiveEngineRenderer::Camera();
    camera.set_perspective(90, camera_perspective_ratio, 1e3, 1e10); // last two args are close and far values
    camera.set_position(position); // initial camera position is at origin (0.0, 0.0, 0.0)

    // Lines are rendered in order, which means a line is described by an index -> index+1
    // Line descriptors -> [Line_1_A, Line_1_B, Line_2_A, Line_2_B]
    std::vector<glm::vec3> lines;
    // Color descriptors -> [Line_1_A, Line_1_B, Line_2_A, Line_2_B]
    std::vector<glm::vec3> line_colors;

    // Creates the blue colored grid
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

    auto ld = new HiveEngineRenderer::LineRenderer(); // Creates the line renderer
    ld->init(); // Initializes line renderer, this line can be checked for failures (it returns a boolean, true=SUCCESS)

    HiveEngineRenderer::TextRenderer consolas("GL_CODE/consolas"); // Creates 2d text renderer
    consolas.init(); // Same thing with line renderer

    // Creating an entity
    HiveEngine::Entity_Deprecated e(glm::vec3(6.0, 0.0, 0.3), 0.5, 100.0);

    // Movement test
    // e should start going left and continue, slowly...
    e.apply_force(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-0.3, 0.0, 0.0), true);

    // Rotation test
    // e should start rotating on x+ axis, fast...
    // we use symmetrical force to prevent any other unnecessary moves
    e.apply_force(glm::vec3(0.0, 0.0, e.get_radius()), glm::vec3(0.0, 2, 0.0), true);
    e.apply_force(glm::vec3(0.0, 0.0, -e.get_radius()), glm::vec3(0.0, -2, 0.0), true);

    while (!glfwWindowShouldClose(window)) {
        // reverse the rotation slowly
        e.apply_force(glm::vec3(0.0, 0.0, e.get_radius()), glm::vec3(0.0, -0.002, 0.0), true);
        e.apply_force(glm::vec3(0.0, 0.0, -e.get_radius()), glm::vec3(0.0, 0.002, 0.0), true);

        camera.set_perspective(90, camera_perspective_ratio, 0.01, 1e5); // set new perspective 90 degrees, camera aspect, 0.01 as near, 1*10^5 as far
        auto view = camera.get_view(); // Get view matrix -> glm::mat4
        camera.get_user_input(window, false); // Gets the camera input from keyboard.
        /***********   CAMERA CONTROLS   ***********
         *
         * W = FORWARDS
         * S = BACKWARDS
         * A = LEFT
         * D = RIGHT
         *
         * E = CLOCKWISE ROTATION
         * Q = COUNTERCLOCKWISE ROTATTION
         *
         * SPACE = UP
         * SHIFT = DOWN
         *
         * FWD ARROW = LOOK UP
         * BCK ARROW = LOOK DOWN
         * LEFT ARROW = LOOK LEFT
         * RIGHT ARROR = LOOK RIGHT
         *
         * = KEY = INCREASE TRAVERSE SPEED
         * - KEY = DECREASE TRAVERSE SPEED
         * [ KEY = INCREASE ROTATION SPEED
         * ] KEY = DECREASE ROTATION SPEED
         *********************************************/

        // Follow the object and reset its position if it goes to left too much.
        view = glm::translate(view, -e.get_position());
        if(e.get_position().x < -80) e.set_position(glm::vec3(80.0, 0.0, 0.3));

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Clear the screen and paint it to given values. (RGBA)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the depth and color buffer, :| try and see what happens if you dont use it...

        ld->draw(lines.data(), line_colors.data(), lines.size() / 2, view); // draw the lines on screen

        // Rendering text
        // 2d screen is described between -1 to 1 for all axex
        float char_d; // this variable is describing the next line on the screen
        float text_scale = 0.7f;
        char_d = consolas.render("traverse_modifier: " + std::to_string(camera.traverse_modifier),
                -1.0f, 1.0f,
                text_scale,
                text_scale*camera_perspective_ratio);
        char_d = consolas.render("rotation_modifier: " + std::to_string(camera.rotate_modifier),
                -1.0f, char_d, // notice char_d is used here and not above this line.
                text_scale,
                text_scale*camera_perspective_ratio);

        char_d = consolas.render("tot acc: " + HiveEngine::vec3_to_str(e.get_total_angular_acceleration()),
                                 -1.0f, char_d, // notice char_d is used here and not above this line.
                                 text_scale,
                                 text_scale*camera_perspective_ratio);

        // Draw entity oritentation
        glLineWidth(4); // Makes the lines thiccker
        auto line_pair = HiveEngine::generate_entity_line_description(&e, glm::vec3(1.0, 1.0, 0.3));
        ld->draw(line_pair.first.data(), line_pair.second.data(), line_pair.first.size() / 2, view);
        glLineWidth(1);

        // Calculate entity for 1 iteration;
        // This function is virtual, so you can implement your own entity.
        // If you want movement behaviour make sure to call the super version.
        e.step(30);

        glfwSwapBuffers(window); // Swap the screen with the new generated one
        glfwPollEvents(); // check things like keyboard presses and window resizing


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
