//
// Created by calpe on 9/15/2018.
//

#ifndef COPTERSWARM_MOTOR_H
#define COPTERSWARM_MOTOR_H

#include "CommonStructs.h"
#include "../HiveEngine/Entity.h"
#include "Serializable.h"

namespace CopterLib {
    class EnergySource;

    class Motor : public HiveEngine::Entity, public Serializable {
    private:
        float current_throttle;
        float max_power_output;
        float mass;
        float efficiency;
    public:
        Motor(float max_power_output, float efficiency, glm::vec3 pos, float radius, float mass);
        void set_throttle(float throttle);
        SymetricForce get_output(EnergySource* es, float de_amp);
        std::vector<char> serialize() override;
    };
}


#endif //COPTERSWARM_MOTOR_H
