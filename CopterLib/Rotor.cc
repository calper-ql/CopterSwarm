//
// Created by calpe on 9/13/2018.
//

#include "Rotor.h"

namespace CopterLib {
    Rotor::Rotor(float ld_ratio, float drag_constant, bool closed_rotor, glm::vec3 pos, float radius, float mass) : Entity_Deprecated(pos, radius, mass){
        this->ld_ratio = ld_ratio;
        this->drag_constant = drag_constant;
        this->closed_rotor = closed_rotor;
    }


    HiveEngine::EntityStepOutput Rotor::step(unsigned steps_per_second){
        auto right_leverage = glm::vec3(get_radius(), 0.0f, 0.0f);
        auto left_leverage = glm::vec3(-get_radius(), 0.0f, 0.0f);

        auto throw_vec_right = this->calculate_throw_acceleration(right_leverage, !this->closed_rotor) / 2.0f;
        auto throw_vec_left = this->calculate_throw_acceleration(left_leverage, !this->closed_rotor) / 2.0f;

        auto right_magnitude = glm::length(throw_vec_right);
        right_magnitude *= right_magnitude / 2.0f;
        auto left_magnitude = glm::length(throw_vec_left);
        left_magnitude *= left_magnitude / 2.0f;

        this->apply_force(right_leverage, glm::normalize(-throw_vec_right) * right_magnitude, true);
        this->apply_force(left_leverage, glm::normalize(-throw_vec_left) * left_magnitude, true);
        this->apply_force(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0 * (ld_ratio * (right_magnitude+left_magnitude))), true); // up force

        return Entity_Deprecated::step(steps_per_second);
    }
}