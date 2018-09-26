//
// Created by calpe on 9/15/2018.
//

#ifndef COPTERSWARM_COPTER_H
#define COPTERSWARM_COPTER_H

#include "Motor.h"
#include "EnergySource.h"
#include "Rotor.h"
#include <vector>
#include "Serializable.h"

namespace CopterLib {
    class Copter : public HiveEngine::Entity, public Serializable {
    private:
        std::vector<std::pair<Motor*, Rotor*>> actuators;
        EnergySource* energy_source;

    public:
        Copter(glm::vec3 pos, float radius, float mass);

        void add_actuator(std::pair<Motor*, Rotor*> actuator);

        void set_energy_source(EnergySource* energy_source);

        virtual void step_motor(unsigned steps_per_second);

        virtual HiveEngine::EntityStepOutput step(unsigned steps_per_second);

        bool set_throttle(std::vector<float> values);

        std::vector<char> serialize() override;
    };
}


#endif //COPTERSWARM_COPTER_H
