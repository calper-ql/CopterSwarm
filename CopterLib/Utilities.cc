//
// Created by calpe on 9/17/2018.
//

#include "Utilities.h"


namespace CopterLib {

    /*
     *
     *                 COMMAND
     *
     */

    std::vector<char> create_command_header(std::string command) {
        std::vector<char> str;
        unsigned iter = 0;
        auto header_size = (unsigned)command.length();
        str.resize(header_size + sizeof(unsigned));

        // INTERNAL
        std::memcpy(str.data() + iter, &header_size, sizeof(header_size));
        iter += sizeof(header_size);

        std::memcpy(str.data() + iter, command.data(), header_size);
        iter += header_size;
        return str;
    }

    std::vector<char> add_unsigned_to_command(std::vector<char> command, unsigned value) {
        auto iter = (unsigned) command.size();
        command.resize(command.size() + sizeof(value));
        std::memcpy(command.data() + iter, &value, sizeof(value));
        return command;
    }

    std::vector<char> add_float_to_command(std::vector<char> command, float value) {
        auto iter = (unsigned) command.size();
        command.resize(command.size() + sizeof(value));
        std::memcpy(command.data() + iter, &value, sizeof(value));
        return command;
    }

    std::vector<char> add_vec3_to_command(std::vector<char> command, glm::vec3 value) {
        auto iter = (unsigned) command.size();
        command.resize(command.size() + sizeof(value));
        std::memcpy(command.data() + iter, &value, sizeof(value));
        return command;
    }

    std::vector<char> add_mat3_to_command(std::vector<char> command, glm::mat3 value) {
        auto iter = (unsigned) command.size();
        command.resize(command.size() + sizeof(value));
        std::memcpy(command.data() + iter, &value, sizeof(value));
        return command;
    }

    std::vector<char> add_bool_to_command(std::vector<char> command, bool value) {
        auto iter = (unsigned) command.size();
        command.resize(command.size() + sizeof(value));
        std::memcpy(command.data() + iter, &value, sizeof(value));
        return command;
    }



    /*
     *
     *                 UNPACK
     *
     */

    std::string read_command(std::vector<char> command) {
        unsigned header_size = *(unsigned *)command.data();
        std::string command_str = std::string(command.data() + sizeof(header_size), header_size);
        return command_str;
    }

    std::string unpack_command(std::vector<char>& command) {
        unsigned header_size = *(unsigned *)command.data();
        std::string command_str = std::string(command.data() + sizeof(header_size), header_size);
        command.erase(command.begin(), command.begin() + sizeof(unsigned) + header_size);
        return command_str;
    }

    unsigned unpack_unsigned(std::vector<char> &command) {
        unsigned value = *(unsigned *)command.data();
        command.erase(command.begin() + sizeof(unsigned));
        return value;
    }

    float unpack_float(std::vector<char> &command) {
        float value = *(float *)command.data();
        command.erase(command.begin(), command.begin() + sizeof(float));
        return value;
    }

    glm::vec3 unpack_vec3(std::vector<char> &command) {
        glm::vec3 value = *(glm::vec3 *)command.data();
        command.erase(command.begin(), command.begin() + sizeof(glm::vec3));
        return value;
    }

    glm::mat3 unpack_mat3(std::vector<char> &command) {
        glm::mat3 value = *(glm::mat3 *)command.data();
        command.erase(command.begin(), command.begin() + sizeof(glm::mat3));
        return value;
    }

    bool unpack_bool(std::vector<char> &command) {
        bool value = *(bool *)command.data();
        command.erase(command.begin(), command.begin() + sizeof(bool));
        return value;
    }


}