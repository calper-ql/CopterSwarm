//
// Created by calpe on 9/13/2018.
//

#include "Rotor.h"

#include "../HiveEngine/Utilities.h"
#include "Utilities.h"
#include <iostream>

namespace CopterLib {
    Rotor::Rotor(float ld_ratio, float drag_constant, bool closed_rotor, glm::vec3 pos, float radius, float mass) : Entity(pos, radius, mass){
        this->ld_ratio = ld_ratio;
        this->drag_constant = drag_constant;
        this->closed_rotor = closed_rotor;
    }


    HiveEngine::EntityStepOutput Rotor::step(unsigned steps_per_second){
        float deamplify_ratio = 1.0f / (float)steps_per_second;
        auto cm = get_rotation_matrix();
        auto right_leverage = glm::vec3(get_radius() / 2.0f, 0.0f, 0.0f);
        auto left_leverage = glm::vec3(-get_radius() / 2.0f, 0.0f, 0.0f);

        auto throw_vec_right = this->calculate_throw_vector(right_leverage, !this->closed_rotor);
        auto throw_vec_left = this->calculate_throw_vector(left_leverage, !this->closed_rotor);

        throw_vec_left *= deamplify_ratio;
        throw_vec_right *= deamplify_ratio;

        auto right_magnitude = glm::length(throw_vec_right);
        right_magnitude *= right_magnitude;
        auto left_magnitude = glm::length(throw_vec_left);
        left_magnitude *= left_magnitude;

        right_magnitude *= drag_constant;
        left_magnitude *= drag_constant;

        //std::cout << " " << HiveEngine::vec3_to_str(glm::normalize(-throw_vec_right)) << " " << right_magnitude << std::endl;

        this->apply_force(right_leverage, glm::normalize(-throw_vec_right) * right_magnitude, true);
        this->apply_force(left_leverage, glm::normalize(-throw_vec_left) * left_magnitude, true);
        this->apply_force(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0 * (ld_ratio * (right_magnitude+left_magnitude) * deamplify_ratio)), true); // up force

        return Entity::step(steps_per_second);
    }

    std::vector<char> Rotor::serialize() {
        std::vector<char> str = create_command_header("Rotor");
        str = add_float_to_command(str, ld_ratio);
        str = add_float_to_command(str, drag_constant);
        str = add_bool_to_command(str, closed_rotor);
        str = add_vec3_to_command(str, get_position());
        str = add_float_to_command(str, get_radius());
        str = add_float_to_command(str, get_mass());
        return str;
    }

}