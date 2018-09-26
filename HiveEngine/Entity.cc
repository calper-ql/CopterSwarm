//
// Created by calpe on 9/15/2018.
//

#include <iostream>
#include "Entity.h"
#include "Utilities.h"

namespace HiveEngine {

    Entity::Entity(glm::vec3 position, float radius, float mass) {
        this->position = position;
        this->radius = radius;
        this->mass = mass;
        this->parent = nullptr;
        this->velocity = glm::vec3(0.0, 0.0, 0.0);
        this->rotation_matrix = glm::mat3(1.0);
        this->total_torque_counter = glm::vec3(0.0, 0.0, 0.0);
        this->moment_of_inertia = glm::mat3(2.0f/5.0f * mass * radius * radius);
        this->calculated_moment_of_inertia = this->moment_of_inertia;
        this->torque_resistance = glm::vec3(1.0f, 1.0f, 1.0f);
        this->angular_velocity = glm::mat3(1.0);
    }

    Entity::~Entity() {
        for (auto item : children) {
            delete item;
        }
    }

    /* ========= GETTERS & SETTERS =========  */

    const glm::vec3 &Entity::get_position() const {
        return position;
    }

    void Entity::set_position(const glm::vec3 &position) {
        Entity::position = position;
    }

    const glm::vec3 &Entity::get_velocity() const {
        if(parent) return parent->get_velocity();
        return velocity;
    }

    void Entity::set_velocity(const glm::vec3 &velocity) {
        Entity::velocity = velocity;
    }

    float Entity::get_mass() const {
        return mass;
    }

    void Entity::set_mass(float mass) {
        Entity::mass = mass;
    }

    const glm::mat3 &Entity::get_rotation_matrix() const {
        return rotation_matrix;
    }

    void Entity::set_rotation_matrix(const glm::mat3 &rotation_matrix) {
        Entity::rotation_matrix = rotation_matrix;
    }

    const glm::mat3 &Entity::get_moment_of_inertia() const {
        return moment_of_inertia;
    }

    void Entity::set_moment_of_inertia(const glm::mat3 &moment_of_inertia) {
        Entity::moment_of_inertia = moment_of_inertia;
    }

    float Entity::get_radius() const {
        return radius;
    }

    void Entity::set_radius(float radius) {
        Entity::radius = radius;
    }

    Entity *Entity::get_parent() const {
        return parent;
    }

    void Entity::set_parent(Entity *parent) {
        Entity::parent = parent;
    }

    const std::vector<Force> &Entity::get_applied_forces() const {
        return applied_forces;
    }

    const EntityStepOutput &Entity::get_last_eso() const {
        return last_eso;
    }

    void Entity::set_last_eso(const EntityStepOutput &last_eso) {
        Entity::last_eso = last_eso;
    }

    /* ========= FUNCTIONS =========  */

    EntityStepOutput Entity::step(unsigned steps_per_second) {
        float deamplify_ratio = 1.0f / (float)steps_per_second;
        EntityStepOutput eso;
        eso.moment_of_inertia = moment_of_inertia;
        eso.mass = mass;
        CentralMass mc;
        if(parent == nullptr) {
            mc = calculate_central_mass();
        }

        for (auto child: children) {
            auto cso = child->step(steps_per_second);
            eso.torque += cso.torque;
            eso.mass += cso.mass;
            glm::mat3 child_moi = glm::mat3(1.0);
            auto c_pos = child->get_position(); c_pos *= c_pos;
            child_moi[0][0] = child->mass * (c_pos.y + c_pos.z);
            child_moi[1][1] = child->mass * (c_pos.x + c_pos.z);
            child_moi[2][2] = child->mass * (c_pos.x + c_pos.y);
            eso.moment_of_inertia += child_moi + cso.moment_of_inertia;
        }

        if(parent == nullptr) {
            glm::mat3 child_moi = glm::mat3(1.0);
            auto c_pos = -mc.position; c_pos *= c_pos;
            child_moi[0][0] = mass * (c_pos.y + c_pos.z);
            child_moi[1][1] = mass * (c_pos.x + c_pos.z);
            child_moi[2][2] = mass * (c_pos.x + c_pos.y);
            eso.moment_of_inertia = child_moi + eso.moment_of_inertia;
        }

        //eso.moment_of_inertia = glm::mat3(2.0f/5.0f * eso.mass * 10 * radius * radius);

        glm::vec3 dt = {0.0f, 0.0f, 0.0f};
        for(auto f: this->applied_forces){
            glm::vec3 force_vector = f.force;
            glm::vec3 leverage;

            if(parent == nullptr) leverage = this->rotation_matrix * (-mc.position+f.leverage);
            else leverage = this->rotation_matrix * f.leverage;

            if(f.is_relative){
                force_vector = this->rotation_matrix * force_vector;
            }

            glm::vec3 torque_vector = glm::cross(leverage, force_vector);
            eso.force += force_vector;

            if(parent) eso.torque += torque_vector * glm::abs(torque_resistance - 1.0f) * deamplify_ratio;
            else eso.torque += torque_vector * deamplify_ratio;

            if(parent) parent->apply_force(position, force_vector, f.is_relative);
        }

        total_torque_counter += eso.torque;

        auto total_w = total_torque_counter / eso.moment_of_inertia;
        calculated_moment_of_inertia = eso.moment_of_inertia;

        angular_velocity = glm::mat3(1.0f);
        angular_velocity *= generate_rotation_matrix('x', total_w[0]);
        angular_velocity *= generate_rotation_matrix('y', total_w[1]);
        angular_velocity *= generate_rotation_matrix('z', total_w[2]);

        last_eso = eso;

        rotation_matrix = rotation_matrix * angular_velocity;
        if(parent == nullptr) {
            velocity += (eso.force / mc.mass);
            auto shift = (angular_velocity * -mc.position) + mc.position;
            position += shift + deamplify_ratio * velocity;
            eso.central_mass = shift;
        }

        applied_forces.clear();

        return eso;
    }

    float Entity::calculate_total_mass() {
        float m = this->mass;
        for (const auto &item : this->children) {
            m += item->calculate_total_mass();
        }
        return m;
    }

    void Entity::add_child(Entity *c) {
        children.push_back(c);
        c->parent = this;
    }

    glm::mat3 Entity::calculate_rotation_matrix() {
        if(parent == nullptr) return rotation_matrix;
        return parent->calculate_rotation_matrix() * rotation_matrix;
    }

    glm::vec3 Entity::calculate_throw_vector(glm::vec3 relative_point, bool parent_supported) {
        if(parent && parent_supported){
            return parent->calculate_throw_vector(position, true)
                   + calculate_rotation_matrix()
                   * glm::cross(total_torque_counter/calculated_moment_of_inertia, relative_point);
        }
        return glm::cross(total_torque_counter/calculated_moment_of_inertia, relative_point);
    }

    glm::vec3 Entity::calculate_relative_position() {
        if(parent == nullptr) return glm::vec3(0.0f, 0.0f, 0.0f);
        return parent->calculate_relative_position() + parent->calculate_rotation_matrix() * position;
    }

    glm::vec3 Entity::calculate_position() {
        if(parent == nullptr){
            return position;
        }
        return parent->calculate_position() + parent->calculate_rotation_matrix() * position;
    }

    void Entity::apply_force(glm::vec3 leverage, glm::vec3 force, bool is_relative) {
        if(glm::length(force) > 0.0) applied_forces.emplace_back(leverage, force, is_relative);
    }

    void Entity::get_all_children(std::vector<Entity *> *list) {
        for (Entity* item : children) {
            list->push_back(item);
            item->get_all_children(list);
        }
    }

    CentralMass Entity::calculate_central_mass() {
        CentralMass cm;
        std::vector<Entity*> list;
        get_all_children(&list);
        cm.mass = mass;
        for(auto c: list){
            if(c) {
                cm.position += c->calculate_relative_position() * c->mass;
                cm.mass += c->mass;
            }
        }
        cm.position /= cm.mass;
        return cm;
    }

    void Entity::set_torque_resistance(glm::vec3 percentages) {
        this->torque_resistance = percentages;
    }

    void Entity::erase_children_vector() {
        children.clear();
    }


}