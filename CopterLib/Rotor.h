//
// Created by calpe on 9/13/2018.
//

#ifndef COPTERSWARM_ROTOR_H
#define COPTERSWARM_ROTOR_H

#include "../HiveEngine/Entity.h"

namespace CopterLib {
    class Rotor : public HiveEngine::Entity {
    private:
        float ld_ratio; // lift drag ratio
        float drag_constant; // d = coef * density * vel_squared/2 * area,   drag_constant = coef * density * area
        bool closed_rotor;
    public:
        Rotor(float ld_ratio, float drag_constant, bool closed_rotor, glm::vec3 pos, float radius, float mass);
        virtual HiveEngine::EntityStepOutput step(unsigned steps_per_second);
    };
}


#endif //COPTERSWARM_ROTOR_H
