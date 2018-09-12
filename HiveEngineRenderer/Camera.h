//
// Created by Can Alper on 1/3/18.
//

#ifndef __HIVEENGINE_RENDERER_CAMERA_H
#define __HIVEENGINE_RENDERER_CAMERA_H

#include "../glm/vec3.hpp"
#include "../glm/mat4x4.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/quaternion.hpp"
#include "../glm/gtc/matrix_access.hpp"
#include "opengl_settings.h"
#include <iostream>

namespace HiveEngineRenderer {
    class Camera {
    private:
        glm::dvec3 position;
        glm::dquat orientation;
        glm::dmat4 perspective;

    public:
        double traverse_modifier;
        double rotate_modifier;

        Camera();

        glm::dvec3 get_position();

        glm::dquat get_orientation();

        glm::dmat4 get_perspective();

        void set_perspective(float fov, float ratio, float near, float far);

        void set_position(glm::dvec3 position);

        void rotate(double angle, double x, double y, double z);

        void yaw(float angle);

        void pitch(float angle);

        void roll(float angle);

        glm::mat4 get_view();

        void get_user_input(GLFWwindow *window, bool print);

        void look_at(glm::vec3 point);
    };
}


#endif //HIVEENGINE_CAMERA_H
