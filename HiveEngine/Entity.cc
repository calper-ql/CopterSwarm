//
// Created by calper on 9/11/2018.
//

#include "Entity.h"
#include <iostream>

namespace HiveEngine {
    Entity::Entity(glm::vec3 position, float radius, float mass) {
        this->position = position;
        this->radius = radius;
        this->mass = mass;
        this->parent = nullptr;
        this->velocity = glm::vec3(0.0, 0.0, 0.0);
        this->rotation_matrix = glm::mat3(1.0);
        this->angular_velocity = glm::mat3(1.0);
    }

    Entity::~Entity() {
        for(auto c: this->children){
            delete c;
        }
    }

    float Entity::calculate_total_mass() {
        float m = this->mass;
        for (const auto &item : this->children) {
            m += item->calculate_total_mass();
        }
        return m;
    }

    float Entity::calculate_total_radius() {
        float r = this->radius;
        for (const auto &item : this->children) {
            float c_r = item->calculate_total_radius(this->position);
            if(c_r > r) r = c_r;
        }
        return r;
    }

    float Entity::calculate_total_radius(glm::vec3 origin) {
        float max_distance = glm::distance(origin, position) + this->radius;
        for (const auto &item : this->children) {
            float c_r = item->calculate_total_radius(this->position);
            if(c_r > max_distance) max_distance = c_r;
        }
        return max_distance;
    }

    glm::vec3 Entity::calculate_position() {
        if(parent == nullptr) return position;
        return parent->calculate_position() + parent->rotation_matrix * position;
    }

    glm::mat3 Entity::calculate_rotation_matrix() {
        if(parent == nullptr) return rotation_matrix;
        return parent->calculate_rotation_matrix() * rotation_matrix;
    }

    void Entity::apply_force(glm::vec3 leverage, glm::vec3 force, bool is_relative) {
        applied_forces.emplace_back(leverage, force, is_relative);
    }

    std::vector<Force> Entity::get_applied_forces() {
        return applied_forces;
    }

    float Entity::get_radius() {
        return radius;
    }

    void Entity::set_radius(float radius) {
        this->radius = radius;
    }

    std::pair<glm::vec3, glm::vec3> Entity::step() {
        glm::vec3 total_force;
        glm::vec3 total_torque;
        for (const auto &item : this->children) {
            auto k = item->step();
            total_torque -= k.second;
        }

        for(auto f: this->applied_forces){
            glm::vec3 force_vector = f.force;
            glm::vec3 leverage = this->rotation_matrix * f.leverage;
            if(f.is_relative){
                force_vector = this->rotation_matrix * f.force;
            }
            glm::vec3 torque_vector = glm::cross(leverage, force_vector);
            total_force += force_vector;
            total_torque += torque_vector;
        }

        auto total_mass = calculate_total_mass();
        auto I = (2.0/5.0) * total_mass * radius * radius;
        total_torque /= I;
        auto a = generate_rotation_matrix('x', total_torque[0]);
        a = generate_rotation_matrix('y', total_torque[1]) * a;
        a = generate_rotation_matrix('z', total_torque[2]) * a;

        angular_velocity = angular_velocity * a;
        rotation_matrix = rotation_matrix * angular_velocity;

        if(parent) {
            parent->apply_force(position, total_force, false);
        } else {
            velocity += (total_force / total_mass);
            position += velocity;
        }

        applied_forces.clear();
        return {total_force, total_torque};
    }

    glm::vec3 Entity::get_position() {
        return position;
    }

    void Entity::set_position(glm::vec3 position) {
        this->position = position;
    }


    // FORCE CONSTRUCTOR
    Force::Force(const glm::vec3 &leverage, const glm::vec3 &force, bool is_relative) : leverage(leverage),
                                                                                        force(force),
                                                                                        is_relative(is_relative) {}


}

