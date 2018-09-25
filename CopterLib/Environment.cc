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

        if(command == "get_pos"){
            return get_pos(str);
        }

        if(command == "step"){
            return run_step(str);
        }

        if(command == "gravity"){
            return apply_gravity(str);
        }

        if(command == "add_actuator"){
            return add_actuator(str);
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
        auto index = (unsigned)entity_list.size();
        auto special_index = (unsigned)copters.size();
        copters.push_back(c);
        entity_list.push_back(c);

        std::cout << log << std::endl;
        return add_unsigned_to_command(add_unsigned_to_command(create_command_header("Created"), index), special_index);
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
        auto index = (unsigned)entity_list.size();
        auto special_index = (unsigned)motors.size();
        motors.push_back(c);
        entity_list.push_back(c);

        std::cout << log << std::endl;
        return add_unsigned_to_command(add_unsigned_to_command(create_command_header("Created"), index), special_index);
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
        auto index = (unsigned)entity_list.size();
        auto special_index = (unsigned)rotors.size();
        rotors.push_back(c);
        entity_list.push_back(c);

        std::cout << log << std::endl;
        return add_unsigned_to_command(add_unsigned_to_command(create_command_header("Created"), index), special_index);
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
        auto index = (unsigned)entity_list.size();
        auto special_index = (unsigned)energy_sources.size();
        energy_sources.push_back(c);
        entity_list.push_back(c);

        std::cout << log << std::endl;
        return add_unsigned_to_command(add_unsigned_to_command(create_command_header("Created"), index), special_index);
    }

    std::vector<char> Environment::get_pos(std::vector<char> str) {
        unpack_command(str);
        auto index = unpack_unsigned(str);
        if(index >= entity_list.size())
            return add_unsigned_to_command(create_command_header("Failure"), 5);
        auto pos = entity_list[index]->calculate_position();
        std::string log = std::string("returning-> pos: ") + " index: " + std::to_string(index)
                + " pos:" + HiveEngine::vec3_to_str(pos);

        std::cout << log << std::endl;
        return add_vec3_to_command(create_command_header("pos"), pos);
    }

    std::vector<char> Environment::run_step(std::vector<char> str) {
        unpack_command(str);
        auto tick = unpack_unsigned(str);
        if(tick == 0)
            return add_unsigned_to_command(create_command_header("Failure"), 5);
        std::string log = std::string("stepping-> ") + " tick: " + std::to_string(tick);

        for (auto item : copters) {
            item->step(tick);
        }

        std::cout << log << std::endl;
        return create_command_header("done");
    }

    std::vector<char> Environment::apply_gravity(std::vector<char> str) {
        unpack_command(str);
        auto vec = unpack_vec3(str);
        //for (auto item : copters) {
        //    auto mc = item->calculate_central_mass();
        //    item->apply_force(mc.position*item->get_rotation_matrix(), vec * mc.mass, false);
        //}
        for (auto item : entity_list) {
            item->apply_force(glm::vec3(0.0), vec * item->get_mass(), false);
        }
        std::string log = std::string("gravity-> ") + HiveEngine::vec3_to_str(vec);
        std::cout << log << std::endl;
        return create_command_header("done");
    }

    std::vector<char> Environment::add_actuator(std::vector<char> str) {
        unpack_command(str);
        auto copter_id = unpack_unsigned(str);
        auto motor_id = unpack_unsigned(str);
        auto rotor_id = unpack_unsigned(str);

        if(copter_id >= copters.size()) return create_command_header("BoundaryFailure");
        if(motor_id >= motors.size()) return create_command_header("BoundaryFailure");
        if(rotor_id >= rotors.size()) return create_command_header("BoundaryFailure");

        copters[copter_id]->add_actuator({motors[motor_id], rotors[rotor_id]});

        std::string log = std::string("actuator-> ") +
                std::to_string(copter_id) + ": " +
                std::to_string(motor_id) + " -- " +
                std::to_string(rotor_id);

        std::cout << log << std::endl;

        return create_command_header("Success");
    }
}