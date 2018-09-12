//
// Created by Can Alper on 1/3/18.
//

#include "Camera.h"

HiveEngineRenderer::Camera::Camera() {
    orientation = glm::dquat(glm::dvec3(0, 0, 0));
    set_perspective(90, 1, 0.1, 1000.0);
    traverse_modifier = 0.4;
    rotate_modifier = 0.01;
}

glm::dvec3 HiveEngineRenderer::Camera::get_position() {
    return -this->position;
}

glm::dquat HiveEngineRenderer::Camera::get_orientation() {
    return this->orientation;
}

glm::dmat4 HiveEngineRenderer::Camera::get_perspective() {
    return this->perspective;
}

void HiveEngineRenderer::Camera::set_perspective(float fov, float ratio, float near_, float far_) {
    this->perspective = glm::perspective(glm::radians(fov), ratio, near_, far_);
}

void HiveEngineRenderer::Camera::set_position(glm::dvec3 position) {
    this->position = -position;
}

void HiveEngineRenderer::Camera::rotate(double angle, double x, double y, double z) {
    auto a = orientation;
    auto b = glm::angleAxis(angle, glm::dvec3(x, y, z) * a);
    orientation = a * b;
}

void HiveEngineRenderer::Camera::yaw(float angle) {
    this->rotate(angle, 0.0, 1.0, 0.0);
}

void HiveEngineRenderer::Camera::pitch(float angle) {
    this->rotate(angle, 1.0, 0.0, 0.0);
}

void HiveEngineRenderer::Camera::roll(float angle) {
    this->rotate(angle, 0.0, 0.0, 1.0);
}

glm::mat4 HiveEngineRenderer::Camera::get_view() {
    return perspective * glm::translate(glm::mat4_cast(orientation), position);
}

void HiveEngineRenderer::Camera::get_user_input(GLFWwindow *window, bool print) {

    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        traverse_modifier *= 1.01;
        if (traverse_modifier > 1e6) traverse_modifier = 1e6;
        if (print) std::cout << "Camera->traverse_modifier = " << traverse_modifier << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        traverse_modifier /= 1.01;
        if (traverse_modifier < 1e-6) traverse_modifier = 1e-6;
        if (print) std::cout << "Camera->traverse_modifier = " << traverse_modifier << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) {
        rotate_modifier *= 1.01;
        if (rotate_modifier > 0.5) rotate_modifier = 0.5;
        if (print) std::cout << "Camera->rotate_modifier = " << rotate_modifier << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) {
        rotate_modifier /= 1.01;
        if (rotate_modifier < 1e-3) rotate_modifier = 1e-3;
        if (print) std::cout << "Camera->rotate_modifier = " << rotate_modifier << std::endl;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += glm::dvec3(glm::row(glm::mat4_cast(get_orientation()), 2)) * traverse_modifier;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= glm::dvec3(glm::row(glm::mat4_cast(get_orientation()), 2)) * traverse_modifier;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position += glm::dvec3(glm::row(glm::mat4_cast(get_orientation()), 0)) * traverse_modifier;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position -= glm::dvec3(glm::row(glm::mat4_cast(get_orientation()), 0)) * traverse_modifier;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position -= glm::dvec3(glm::row(glm::mat4_cast(get_orientation()), 1)) * traverse_modifier;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        position += glm::dvec3(glm::row(glm::mat4_cast(get_orientation()), 1)) * traverse_modifier;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        pitch(rotate_modifier);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        pitch(-rotate_modifier);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        yaw(-rotate_modifier);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        yaw(rotate_modifier);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        roll(rotate_modifier);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        roll(-rotate_modifier);
    }

}

void HiveEngineRenderer::Camera::look_at(glm::vec3 point) {
    //orientation = glm::dquat(glm::dvec3(0, 0, 0));
    glm::dvec3 up = glm::dvec3(glm::row(glm::mat4_cast(get_orientation()), 1));
}
