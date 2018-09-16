//
// Created by calpe on 9/15/2018.
//

#include "EnergySource.h"


namespace CopterLib {


    EnergySource::EnergySource(float energy, float max_capacity, glm::vec3 pos, float radius, float mass): Entity_Deprecated(pos, radius, mass) {
        this->energy = 0.0;
        deposit_energy(energy);
        this->max_capacity = max_capacity;
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
}