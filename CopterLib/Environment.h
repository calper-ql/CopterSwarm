//
// Created by calpe on 9/18/2018.
//

#ifndef COPTERSWARM_ENVIRONMENT_H
#define COPTERSWARM_ENVIRONMENT_H

#include <string>
#include <iostream>

#include "Copter.h"
#include "../HiveEngine/Utilities.h"
#include "../HiveEngine/Entity.h"
#include "../HiveEngine/CommonStructs.h"

namespace CopterLib {

    class Environment {
    private:

    public:
        std::vector<Copter*> copters;
        std::vector<Motor*> motors;
        std::vector<Rotor*> rotors;
        std::vector<EnergySource*> energy_sources;

        std::vector<HiveEngine::Entity*> entity_list;

        Environment();
        ~Environment();

        std::vector<char> command(std::vector<char> str);
        std::vector<char> construct_copter(std::vector<char> str);
        std::vector<char> construct_motor(std::vector<char> str);
        std::vector<char> construct_rotor(std::vector<char> str);
        std::vector<char> construct_energy_source(std::vector<char> str);


    };

}


#endif //COPTERSWARM_ENVIRONMENT_H
