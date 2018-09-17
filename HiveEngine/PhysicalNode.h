//
// Created by calpe on 9/16/2018.
//

#ifndef COPTERSWARM_PHYSICALNODE_H
#define COPTERSWARM_PHYSICALNODE_H

#include "../glm/glm.hpp"

namespace HiveEngine {


    class PhysicalNode {
    private:
        glm::vec3 position;
        glm::vec3 velocity;
        float mass;

        glm::mat3 rotation_matrix;
        glm::mat3 moment_of_inertia;
    };
}

#endif //COPTERSWARM_PHYSICALNODE_H
