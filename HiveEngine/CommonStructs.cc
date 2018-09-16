//
// Created by calpe on 9/15/2018.
//

#include "CommonStructs.h"

namespace HiveEngine {
    // FORCE CONSTRUCTOR
    Force::Force(const glm::vec3 &leverage, const glm::vec3 &force, bool is_relative) : leverage(leverage),
                                                                                        force(force),
                                                                                        is_relative(is_relative) {}

    Force::Force() {
        leverage = glm::vec3(0.0f, 0.0f, 0.0f);
        force = glm::vec3(0.0f, 0.0f, 0.0f);
        is_relative = false;
    }

    EntityStepOutput::EntityStepOutput() {
        force = glm::vec3(0.0f, 0.0f, 0.0f);
        torque = glm::vec3(0.0f, 0.0f, 0.0f);
        moment_of_inertia = glm::mat3(1.0f);
        central_mass = glm::vec3(0.0f, 0.0f, 0.0f);
        mass = 0.0f;
    }

    CentralMass::CentralMass() {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        mass = 0.0f;
    }
}