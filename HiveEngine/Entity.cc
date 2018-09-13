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
        this->angular_velocity = glm::quat(glm::vec3(0.0, 0.0, 0.0));
        this->angular_acceleration_counter = glm::vec3(0.0, 0.0, 0.0);
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
        auto I = (2.0f/5.0f) * total_mass * radius * radius;
        auto total_w = total_torque / I;

        angular_acceleration_counter += total_w;
        angular_velocity *= glm::angleAxis(total_w[0], glm::vec3(1.0, 0.0, 0.0) * angular_velocity);
        angular_velocity *= glm::angleAxis(total_w[1], glm::vec3(0.0, 1.0, 0.0) * angular_velocity);
        angular_velocity *= glm::angleAxis(total_w[2], glm::vec3(0.0, 0.0, 1.0) * angular_velocity);

        rotation_matrix = rotation_matrix * glm::mat3_cast(angular_velocity);

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

    glm::quat Entity::get_angular_velocity() {
        return angular_velocity;
    }

    glm::vec3 Entity::get_total_angular_acceleration() {
        return angular_acceleration_counter;
    }

    glm::vec3 Entity::calculate_throw_acceleration(glm::vec3 relative_point, bool parent_supported) {
        if(parent && parent_supported){
            return parent->calculate_throw_acceleration(position, true)
            + calculate_rotation_matrix() * glm::cross(get_total_angular_acceleration(), relative_point);
        }
        return calculate_rotation_matrix() * glm::cross(get_total_angular_acceleration(), relative_point);
    }

    glm::mat3 Entity::get_rotation_matrix() {
        return rotation_matrix;
    }

    void Entity::add_child(Entity *c) {
        children.push_back(c);
        c->parent = this;
    }

    glm::vec3 Entity::get_velocity() {
        if(parent) return parent->get_velocity();
        return velocity;
    }


    // FORCE CONSTRUCTOR
    Force::Force(const glm::vec3 &leverage, const glm::vec3 &force, bool is_relative) : leverage(leverage),
                                                                                        force(force),
                                                                                        is_relative(is_relative) {}


}

