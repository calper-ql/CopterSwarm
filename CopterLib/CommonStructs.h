//
// Created by calpe on 9/15/2018.
//

#ifndef COPTERSWARM_COMMONSTRUCTS_H
#define COPTERSWARM_COMMONSTRUCTS_H

#include "../glm/glm.hpp"
#include "../HiveEngine/CommonStructs.h"

namespace CopterLib {
    struct SymetricForce {
        HiveEngine::Force left;
        HiveEngine::Force right;
    };
}

#endif //COPTERSWARM_COMMONSTRUCTS_H
