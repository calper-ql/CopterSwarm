//
// Created by calpe on 9/15/2018.
//

#include "Copter.h"

namespace CopterLib {

    Copter::Copter(glm::vec3 pos, float radius, float mass) : Entity(pos, radius, mass) {
        this->energy_source = nullptr;
    }

    void Copter::add_actuator(std::pair<Motor *, Rotor *> actuator) {
        actuators.push_back(actuator);
        //add_child(actuator.second);
        add_child(actuator.first);
        actuator.first->add_child(actuator.second);
    }

    void Copter::set_energy_source(EnergySource *energy_source) {
        this->energy_source = energy_source;
        add_child(energy_source);
    }

    HiveEngine::EntityStepOutput Copter::step(unsigned steps_per_second) {
        float deamplify_ratio = 1.0f / (float)steps_per_second;
        for(auto p: actuators){
            if(energy_source){
                auto sym_f = p.first->get_output(energy_source, deamplify_ratio);
                p.second->apply_force(sym_f.left.leverage, sym_f.left.force, sym_f.left.is_relative);
                p.second->apply_force(sym_f.right.leverage, sym_f.right.force, sym_f.right.is_relative);
            }
        }
        return Entity::step(steps_per_second);
    }

    bool Copter::set_throttle(std::vector<float> values) {
        if(actuators.size() != values.size()) return false;
        for (size_t i = 0; i < values.size(); i++) {
            actuators[i].first->set_throttle(values[i]);
        }
        return true;
    }

    std::vector<char> Copter::serialize() {
        std::vector<char> str = create_command_header("Copter");
        str = add_vec3_to_command(str, get_position());
        str = add_float_to_command(str, get_radius());
        str = add_float_to_command(str, get_mass());
        return str;
    }

}