//
// Created by calpe on 9/15/2018.
//

#ifndef COPTERSWARM_MOTOR_H
#define COPTERSWARM_MOTOR_H

#include "CommonStructs.h"
#include "../HiveEngine/Entity_Deprecated.h"

namespace CopterLib {
    class EnergySource;

    class Motor : public HiveEngine::Entity_Deprecated {
    private:
        float current_throttle;
        float max_power_output;
        float mass;
    public:
        Motor(float max_power_output, glm::vec3 pos, float radius, float mass);
        void set_throttle(float throttle);
        SymetricForce get_output(EnergySource* es);
    };
}


#endif //COPTERSWARM_MOTOR_H
