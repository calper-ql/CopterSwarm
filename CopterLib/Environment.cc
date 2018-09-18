//
// Created by calpe on 9/18/2018.
//

#include "Environment.h"

namespace CopterLib {


    Environment::Environment() {

    }

    Environment::~Environment() {
        for (auto item : entity_list) {
            item->erase_children_vector();
            delete item;
        }
    }

    std::vector<char> Environment::command(std::vector<char> str) {
        if(str.size() <= 4) return add_unsigned_to_command(create_command_header("Failure"), 100);
        auto command = read_command(str);

        if(command == "Copter"){
            return construct_copter(str);
        }

        if(command == "Motor"){
            return construct_motor(str);
        }

        if(command == "Rotor"){
            return construct_rotor(str);
        }

        if(command == "EnergySource"){
            return construct_energy_source(str);
        }


        return add_unsigned_to_command(create_command_header("Failure"), 1);
    }

    std::vector<char> Environment::construct_copter(std::vector<char> str) {
        auto ct = Copter({0, 0, 0}, 0, 0);
        if(str.size() != ct.serialize().size())
            return add_unsigned_to_command(create_command_header("Failure"), 2);

        unpack_command(str);

        auto vec = unpack_vec3(str);
        auto radius = unpack_float(str);
        auto mass = unpack_float(str);

        std::string log = std::string("Creating-> Copter ")
                          + " pos:" + HiveEngine::vec3_to_str(vec)
                          + " radius:" + std::to_string(radius)
                          + " mass:" + std::to_string(mass);
        auto c = new Copter(vec, radius, mass);
        auto index = (unsigned)copters.size();
        copters.push_back(c);

        std::cout << log << std::endl;
        return add_unsigned_to_command(create_command_header("Created"), index);
    }

    std::vector<char> Environment::construct_motor(std::vector<char> str) {
        auto ct = Motor(0.0, 0.0, {0, 0, 0}, 0, 0);
        if(str.size() != ct.serialize().size())
            return add_unsigned_to_command(create_command_header("Failure"), 3);

        unpack_command(str);

        auto max_power = unpack_float(str);
        auto efficiency = unpack_float(str);
        auto vec = unpack_vec3(str);
        auto radius = unpack_float(str);
        auto mass = unpack_float(str);

        std::string log = std::string("Creating-> Motor ")
                          + " max_power:" + std::to_string(max_power)
                          + " efficiency:" + std::to_string(efficiency)
                          + " pos:" + HiveEngine::vec3_to_str(vec)
                          + " radius:" + std::to_string(radius)
                          + " mass:" + std::to_string(mass);
        auto c = new Motor(max_power, efficiency, vec, radius, mass);
        auto index = (unsigned)motors.size();
        motors.push_back(c);

        std::cout << log << std::endl;
        return add_unsigned_to_command(create_command_header("Created"), index);
    }

    std::vector<char> Environment::construct_rotor(std::vector<char> str) {
        auto ct = Rotor(0.0, 0.0, false, {0, 0, 0}, 0, 0);
        if(str.size() != ct.serialize().size())
            return add_unsigned_to_command(create_command_header("Failure"), 3);

        unpack_command(str);

        auto dl_radio = unpack_float(str);
        auto drag_constant = unpack_float(str);
        auto enclosed = unpack_bool(str);
        auto vec = unpack_vec3(str);
        auto radius = unpack_float(str);
        auto mass = unpack_float(str);

        std::string log = std::string("Creating-> Rotor: ")
                          + " dl_radio:" + std::to_string(dl_radio)
                          + " drag_constant:" + std::to_string(drag_constant)
                          + " enclosed:" + std::to_string(enclosed)
                          + " pos:" + HiveEngine::vec3_to_str(vec)
                          + " radius:" + std::to_string(radius)
                          + " mass:" + std::to_string(mass);
        auto c = new Rotor(dl_radio, drag_constant, enclosed, vec, radius, mass);
        auto index = (unsigned)rotors.size();
        rotors.push_back(c);

        std::cout << log << std::endl;
        return add_unsigned_to_command(create_command_header("Created"), index);
    }

    std::vector<char> Environment::construct_energy_source(std::vector<char> str) {
        auto ct = EnergySource(0.0, 0.0, {0, 0, 0}, 0, 0);
        if(str.size() != ct.serialize().size())
            return add_unsigned_to_command(create_command_header("Failure"), 4);

        unpack_command(str);

        auto energy = unpack_float(str);
        auto capacity = unpack_float(str);
        auto vec = unpack_vec3(str);
        auto radius = unpack_float(str);
        auto mass = unpack_float(str);

        std::string log = std::string("Creating-> EnergySrouce: ")
                + " energy:" + std::to_string(energy)
                + " capacity:" + std::to_string(capacity)
                + " pos:" + HiveEngine::vec3_to_str(vec)
                + " radius:" + std::to_string(radius)
                + " mass:" + std::to_string(mass);
        auto c = new EnergySource(energy, capacity, vec, radius, mass);
        auto index = (unsigned)energy_sources.size();
        energy_sources.push_back(c);

        std::cout << log << std::endl;
        return add_unsigned_to_command(create_command_header("Created"), index);
    }
}