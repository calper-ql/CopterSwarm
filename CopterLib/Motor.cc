//
// Created by calpe on 9/15/2018.
//

#include "Motor.h"
#include "EnergySource.h"

#include <iostream>

namespace CopterLib {

    Motor::Motor(float max_power_output, float efficiency, glm::vec3 pos, float radius, float mass) : Entity(pos, radius, mass) {
        this->max_power_output = max_power_output;
        if(this->max_power_output < 0.0) this->max_power_output = 0.0;
        set_throttle(0.0);
        this->efficiency = efficiency;
        if(this->efficiency < 0.0f) this->efficiency = 0.0f;
        if(this->efficiency > 1.0f) this->efficiency = 1.0f;
    }

    void Motor::set_throttle(float throttle) {
        this->current_throttle = throttle;
        if(this->current_throttle < -1.0f) this->current_throttle = -1.0f;
        if(this->current_throttle > 1.0f) this->current_throttle = 1.0f;
    }

    SymetricForce Motor::get_output(EnergySource *es, float de_amp) {
        SymetricForce sf;
        if(es == nullptr)
            return sf;

        float acquired_energy = es->request_energy(this->max_power_output * abs(this->current_throttle) * de_amp);
        /*  Angular kinetic energy - http://hyperphysics.phy-astr.gsu.edu/hbase/rke.html
         *  w = angular velocity
         *  ke = 1/2 * I * w^2
         *  w = sqrt(2ke/I) -> I is asumed to be 1
         *
         *  Angular work - http://hyperphysics.phy-astr.gsu.edu/hbase/rke.html
         *  W = Fd or W = T * theta where theta is the angular displacement
         */

        //std::cout << acquired_energy << " " << this->max_power_output * abs(this->current_throttle) << std::endl;

        sf.left.leverage.x = -1.0f;
        sf.right.leverage.x = 1.0f;
        sf.left.is_relative = true;
        sf.right.is_relative = true;

        auto w_half = sqrtf(2.0f * acquired_energy) / 2.0f; // divided because of symmetry
        if(this->current_throttle < 0.0) w_half *= -1.0f;
        w_half *= efficiency;
        sf.left.force.y = w_half;
        sf.right.force.y = -w_half;

        return sf;
    }

    std::vector<char> Motor::serialize() {
        std::vector<char> str = create_command_header("Motor");
        str = add_float_to_command(str, max_power_output);
        str = add_float_to_command(str, efficiency);
        str = add_vec3_to_command(str, get_position());
        str = add_float_to_command(str, get_radius());
        str = add_float_to_command(str, get_mass());
        return str;
    }

}
