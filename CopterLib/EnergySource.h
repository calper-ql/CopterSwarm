//
// Created by calpe on 9/15/2018.
//

#ifndef COPTERSWARM_ENERGYSOURCE_H
#define COPTERSWARM_ENERGYSOURCE_H

#include "../HiveEngine/Entity.h"
#include "Serializable.h"

namespace CopterLib {
    class EnergySource : public HiveEngine::Entity, public Serializable {
    private:
        float energy;
        // expressed in joules -- https://en.wikipedia.org/wiki/Energy
        // https://www.rc-electronics-usa.com/battery-electronics-101.html
        // Charge (C) x Voltage (V) = Energy (J)
        // Current (A) x time (seconds) = Charge (C)
        // For any voltage, mAh X voltage x 3.6 = Joules of energy. !!!!

        float max_capacity;
    public:
        EnergySource(float energy, float max_capacity, glm::vec3 pos, float radius, float mass);

        float request_energy(float amount);

        float deposit_energy(float amount);

        bool is_empty();

        float get_available_energy();

        float get_capacity();

        std::vector<char> serialize() override;

    };
}


#endif //COPTERSWARM_ENERGYSOURCE_H
