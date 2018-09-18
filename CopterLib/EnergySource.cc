//
// Created by calpe on 9/15/2018.
//

#include "EnergySource.h"


namespace CopterLib {


    EnergySource::EnergySource(float energy, float max_capacity, glm::vec3 pos, float radius, float mass): Entity(pos, radius, mass) {
        this->energy = 0.0;
        this->max_capacity = max_capacity;
        deposit_energy(energy);
    }

    float EnergySource::request_energy(float amount) {
        if(amount > 0.0f){
            float b_energy = this->energy;
            this->energy = this->energy - amount;
            if(this->energy < 0.0f) this->energy = 0.0f;
            return b_energy - this->energy;
        }
        return 0.0f;
    }

    float EnergySource::deposit_energy(float amount) {
        if(amount > 0.0f){
            this->energy += amount;
            if(this->energy > this->max_capacity) {
                auto excess_energy = this->energy - this->max_capacity;
                this->energy = max_capacity;
                return  excess_energy;
            }
        }
        return 0.0f;
    }


    bool EnergySource::is_empty() {
        return this->energy <= 0.0;
    }

    float EnergySource::get_available_energy() {
        return this->energy;
    }

    float EnergySource::get_capacity() {
        return max_capacity;
    }

    std::vector<char> EnergySource::serialize() {
        std::vector<char> str = create_command_header("EnergySource");
        str = add_float_to_command(str, energy);
        str = add_float_to_command(str, max_capacity);
        str = add_vec3_to_command(str, get_position());
        str = add_float_to_command(str, get_radius());
        str = add_float_to_command(str, get_mass());
        return str;
    }
}