//
// Created by calpe on 9/18/2018.
//

#ifndef COPTERSWARM_SERIALIZABLE_H
#define COPTERSWARM_SERIALIZABLE_H

#include <vector>
#include <cstdio>
#include <cstring>
#include <string>
#include "Utilities.h"

namespace CopterLib {
    class Serializable {
    public:
        // First 4 bytes is length of chars(unsigned), rest is metadata
        virtual std::vector<char> serialize() = 0;
    };
}


#endif //COPTERSWARM_SERIALIZABLE_H
