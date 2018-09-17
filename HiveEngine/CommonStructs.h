//
// Created by calpe on 9/15/2018.
//

#ifndef HIVEENGINE_COMMONSTRUCTS_H
#define HIVEENGINE_COMMONSTRUCTS_H

#include "../glm/glm.hpp"

namespace HiveEngine {
    class Force {
    public:
        glm::vec3 leverage;
        glm::vec3 force;
        bool is_relative;

        Force();
        Force(const glm::vec3 &leverage, const glm::vec3 &force, bool is_relative);
    };

    class EntityStepOutput {
    public:
        glm::vec3 force;
        glm::vec3 torque;
        glm::mat3 moment_of_inertia;
        glm::vec3 central_mass;
        float mass;

        EntityStepOutput();
    };

    class CentralMass {
    public:
        glm::vec3 position;
        float mass;

        CentralMass();
    };

    class BBX {
        // Bounding Box
        glm::vec3 a;
        glm::vec3 b;

        BBX();
    };
}

#endif //HIVEENGINE_COMMONSTRUCTS_H
