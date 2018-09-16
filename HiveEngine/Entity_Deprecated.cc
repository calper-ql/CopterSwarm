//
// Created by calper on 9/11/2018.
//

#include "Entity_Deprecated.h"
#include <iostream>

namespace HiveEngine {
    Entity_Deprecated::Entity_Deprecated(glm::vec3 position, float radius, float mass) {
        this->position = position;
        this->radius = radius;
        this->mass = mass;
        this->parent = nullptr;
        this->velocity = glm::vec3(0.0, 0.0, 0.0);
        this->rotation_matrix = glm::mat3(1.0);
        this->angular_velocity = glm::quat(glm::vec3(0.0, 0.0, 0.0));
        this->angular_acceleration_counter = glm::vec3(0.0, 0.0, 0.0);
        this->moment_of_inertia = glm::mat3(2.0f/5.0f * mass * radius * radius);
        this->calculated_moi = moment_of_inertia;
    }

    Entity_Deprecated::~Entity_Deprecated() {
        for(auto c: this->children){
            delete c;
        }
    }

    float Entity_Deprecated::calculate_total_mass() {
        float m = this->mass;
        for (const auto &item : this->children) {
            m += item->calculate_total_mass();
        }
        return m;
    }

    float Entity_Deprecated::calculate_total_radius() {
        float r = this->radius;
        for (const auto &item : this->children) {
            float c_r = item->calculate_total_radius(this->position);
            if(c_r > r) r = c_r;
        }
        return r;
    }

    float Entity_Deprecated::calculate_total_radius(glm::vec3 origin) {
        float max_distance = glm::distance(origin, position) + this->radius;
        for (const auto &item : this->children) {
            float c_r = item->calculate_total_radius(this->position);
            if(c_r > max_distance) max_distance = c_r;
        }
        return max_distance;
    }

    glm::vec3 Entity_Deprecated::calculate_position() {
        if(parent == nullptr) return position;
        return parent->calculate_position() + parent->rotation_matrix * position;
    }

    glm::mat3 Entity_Deprecated::calculate_rotation_matrix() {
        if(parent == nullptr) return rotation_matrix;
        return parent->calculate_rotation_matrix() * rotation_matrix;
    }

    void Entity_Deprecated::apply_force(glm::vec3 leverage, glm::vec3 force, bool is_relative) {
        applied_forces.emplace_back(leverage, force, is_relative);
    }

    std::vector<Force> Entity_Deprecated::get_applied_forces() {
        return applied_forces;
    }

    float Entity_Deprecated::get_radius() {
        return radius;
    }

    void Entity_Deprecated::set_radius(float radius) {
        this->radius = radius;
    }

    EntityStepOutput Entity_Deprecated::step(unsigned steps_per_second) {
        if(parent == nullptr){

        }

        EntityStepOutput eso;
        float deamplify_ratio = 1.0f / (float)steps_per_second;
        glm::vec3 total_force;
        glm::vec3 total_torque;
        eso.moment_of_inertia = moment_of_inertia;

        for (const auto &item : this->children) {
            auto k = item->step(steps_per_second);
            total_torque += k.torque;
            glm::mat3 child_moi = glm::mat3(1.0);
            auto c_pos = item->get_position();
            c_pos *= c_pos;
            child_moi[0][0] = item->mass * (c_pos.y + c_pos.z);
            child_moi[1][1] = item->mass * (c_pos.x + c_pos.z);
            child_moi[2][2] = item->mass * (c_pos.x + c_pos.y);
            eso.moment_of_inertia *= k.moment_of_inertia + child_moi;
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

        if(parent == nullptr) {
            std::cout << "-----" << std::endl;
            std::cout << vec3_to_str(eso.moment_of_inertia[0]) << std::endl;
            std::cout << vec3_to_str(eso.moment_of_inertia[1]) << std::endl;
            std::cout << vec3_to_str(eso.moment_of_inertia[2]) << std::endl;
        }

        auto total_mass = calculate_total_mass();
        angular_acceleration_counter += total_torque;
        auto total_w = angular_acceleration_counter / eso.moment_of_inertia;
        calculated_moi = eso.moment_of_inertia;

        angular_velocity = glm::mat3(1.0f);
        angular_velocity *= glm::angleAxis(total_w[0] * deamplify_ratio, glm::vec3(1.0, 0.0, 0.0) * angular_velocity);
        angular_velocity *= glm::angleAxis(total_w[1] * deamplify_ratio, glm::vec3(0.0, 1.0, 0.0) * angular_velocity);
        angular_velocity *= glm::angleAxis(total_w[2] * deamplify_ratio, glm::vec3(0.0, 0.0, 1.0) * angular_velocity);

        rotation_matrix = rotation_matrix * glm::mat3_cast(angular_velocity);

        if(parent) {
            parent->apply_force(position, total_force, false);
        } else {
            velocity += (total_force / total_mass);
            position += deamplify_ratio * velocity;
        }

        applied_forces.clear();
        eso.force = total_force;
        eso.torque = total_torque;
        return eso;
    }

    glm::vec3 Entity_Deprecated::get_position() {
        return position;
    }

    void Entity_Deprecated::set_position(glm::vec3 position) {
        this->position = position;
    }

    glm::quat Entity_Deprecated::get_angular_velocity() {
        return angular_velocity;
    }

    glm::vec3 Entity_Deprecated::get_total_angular_acceleration() {
        return angular_acceleration_counter / calculated_moi;
    }

    glm::vec3 Entity_Deprecated::calculate_throw_acceleration(glm::vec3 relative_point, bool parent_supported) {
        if(parent && parent_supported){
            return parent->calculate_throw_acceleration(position, true)
            + calculate_rotation_matrix() * glm::cross(get_total_angular_acceleration(), relative_point);
        }
        return calculate_rotation_matrix() * glm::cross(get_total_angular_acceleration(), relative_point);
    }

    glm::mat3 Entity_Deprecated::get_rotation_matrix() {
        return rotation_matrix;
    }

    void Entity_Deprecated::add_child(Entity_Deprecated *c) {
        children.push_back(c);
        c->parent = this;
    }

    std::vector<Entity_Deprecated*> Entity_Deprecated::get_children(){
        return children;
    }

    glm::vec3 Entity_Deprecated::get_velocity() {
        if(parent) return parent->get_velocity();
        return velocity;
    }

    void Entity_Deprecated::set_velocity(glm::vec3 velocity) {
        this->velocity = velocity;
    }

    void Entity_Deprecated::set_mass(float mass) {
        this->mass = mass;
    }

    float Entity_Deprecated::get_mass() {
        return mass;
    }

    void Entity_Deprecated::set_rotation_matrix(glm::mat3 rm) {
        this->rotation_matrix = rm;
    }

    glm::mat3 Entity_Deprecated::get_moment_of_inertia() {
        return this->moment_of_inertia;
    }
}

