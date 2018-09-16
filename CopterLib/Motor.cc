//
// Created by calpe on 9/15/2018.
//

#include "Motor.h"
#include "EnergySource.h"

namespace CopterLib {

    Motor::Motor(float max_power_output, glm::vec3 pos, float radius, float mass) : Entity_Deprecated(pos, radius, mass) {
        this->max_power_output = max_power_output;
        if(this->max_power_output < 0.0) this->max_power_output = 0.0;
        set_throttle(0.0);
    }

    void Motor::set_throttle(float throttle) {
        this->current_throttle = throttle;
        if(this->current_throttle < -1.0f) this->current_throttle = -1.0f;
        if(this->current_throttle > 1.0f) this->current_throttle = 1.0f;
    }

    SymetricForce Motor::get_output(EnergySource *es) {
        SymetricForce sf;
        if(es == nullptr)
            return sf;

        float acquired_energy = es->request_energy(this->max_power_output * abs(this->current_throttle));
        /*  Angular kinetic energy - http://hyperphysics.phy-astr.gsu.edu/hbase/rke.html
         *  w = angular velocity
         *  ke = 1/2 * I * w^2
         *  w = sqrt(2ke/I) -> I is asumed to be 1
         *
         *  Angular work - http://hyperphysics.phy-astr.gsu.edu/hbase/rke.html
         *  W = Fd or W = T * theta where theta is the angular displacement
         */


        sf.left.leverage.x = -1.0f;
        sf.right.leverage.x = 1.0f;
        sf.left.is_relative = true;
        sf.right.is_relative = true;

        auto w_half = sqrtf(2*acquired_energy) / 2.0f; // divided because of symmetry
        if(this->current_throttle < 0.0) w_half *= -1.0f;
        sf.left.force.y = w_half;
        sf.right.force.y = -w_half;

        return sf;
    }

}