//
// Created by calpe on 9/13/2018.
//

#ifndef COPTERSWARM_ROTOR_H
#define COPTERSWARM_ROTOR_H

#include "../HiveEngine/Entity.h"

namespace CopterLib {
    class Rotor : public HiveEngine::Entity {
    private:
        float drag_to_lift_ratio;
    public:
        Rotor(float drag_to_lift_ratio, glm::vec3 pos, float radius, float mass);
        virtual void apply_force(glm::vec3 leverage, glm::vec3 force, bool is_relative);
        virtual void step();
    };
}


#endif //COPTERSWARM_ROTOR_H
