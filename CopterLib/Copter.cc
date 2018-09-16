//
// Created by calpe on 9/15/2018.
//

#include "Copter.h"

namespace CopterLib {

    Copter::Copter(glm::vec3 pos, float radius, float mass) : Entity_Deprecated(pos, radius, mass) {
        this->core_mass = mass;
        this->energy_source = nullptr;
    }

    void Copter::add_actuator(std::pair<Motor *, Rotor *> actuator) {
        actuators.push_back(actuator);
        add_child(actuator.second);
        add_child(actuator.first);
        calculate_new_mass();
    }

    void Copter::set_energy_source(EnergySource *energy_source) {
        this->energy_source = energy_source;
        calculate_new_mass();
    }

    HiveEngine::EntityStepOutput Copter::step(unsigned steps_per_second) {
        for(auto p: actuators){
            auto sym_f = p.first->get_output(energy_source);
            p.second->apply_force(sym_f.left.leverage, sym_f.left.force, sym_f.left.is_relative);
            p.second->apply_force(sym_f.right.leverage, sym_f.right.force, sym_f.right.is_relative);
        }

        return Entity_Deprecated::step(steps_per_second);
    }

    bool Copter::set_throttle(std::vector<float> values) {
        if(actuators.size() != values.size()) return false;
        for (size_t i = 0; i < values.size(); i++) {
            actuators[i].first->set_throttle(values[i]);
        }
        return true;
    }

    void Copter::calculate_new_mass() {
        auto tm = core_mass;
        for(auto p: actuators){
            tm += p.first->get_mass();
        }
        if(energy_source) tm += energy_source->get_mass();
        set_mass(tm);
    }
}