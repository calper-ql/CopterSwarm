//
// Created by calpe on 9/17/2018.
//

#ifndef COPTERSWARM_UTILITIES_H
#define COPTERSWARM_UTILITIES_H

#include <vector>
#include <string>
#include <cstring>
#include "../HiveEngine/CommonStructs.h"

namespace CopterLib {
    std::vector<char> create_command_header(std::string command);
    std::vector<char> add_unsigned_to_command(std::vector<char> command, unsigned value);
    std::vector<char> add_float_to_command(std::vector<char> command, float value);
    std::vector<char> add_vec3_to_command(std::vector<char> command, glm::vec3 value);
    std::vector<char> add_mat3_to_command(std::vector<char> command, glm::mat3 value);
    std::vector<char> add_bool_to_command(std::vector<char> command, bool value);

    std::string read_command(std::vector<char> command);
    std::string unpack_command(std::vector<char>& command);
    unsigned unpack_unsigned(std::vector<char>& command);
    float unpack_float(std::vector<char>& command);
    glm::vec3 unpack_vec3(std::vector<char>& command);
    glm::mat3 unpack_mat3(std::vector<char>& command);
    bool unpack_bool(std::vector<char>& command);
}


#endif //COPTERSWARM_UTILITIES_H
