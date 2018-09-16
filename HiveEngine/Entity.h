//
// Created by calpe on 9/15/2018.
//

#ifndef HIVEENGINE_ENTITY_H
#define HIVEENGINE_ENTITY_H

#include <vector>
#include "../glm/glm.hpp"
#include "../glm/gtc/quaternion.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include "CommonStructs.h"

namespace HiveEngine {
    class Entity {
    private:
        glm::vec3 position; // expressed in meters
        glm::vec3 velocity; // expressed in meters per second
        float mass; // expressed in kilograms

        glm::mat3 rotation_matrix; // expressed in radians
        glm::vec3 total_torque_counter; // expressed inn radians
        glm::mat3 moment_of_inertia;

        float radius; // expressed in meters

        glm::mat3 calculated_moment_of_inertia;

        Entity* parent; // parent link
        std::vector<Entity*> children;

        std::vector<Force> applied_forces;

        EntityStepOutput last_eso;

        /* ========= Internal Functions =========  */
        glm::vec3 calculate_relative_position();

    public:
        Entity(glm::vec3 position, float radius, float mass);



        /* ========= Functions =========  */
        virtual EntityStepOutput step(unsigned steps_per_second);

        float calculate_total_mass();

        void add_child(Entity* c);

        glm::mat3 calculate_rotation_matrix();

        glm::vec3 calculate_throw_vector(glm::vec3 relative_point, bool parent_supported);

        glm::vec3 calculate_position();

        virtual void apply_force(glm::vec3 leverage, glm::vec3 force, bool is_relative);

        void get_all_children(std::vector<Entity*> *list);

        CentralMass calculate_central_mass();

        /* ========= GETTERS & SETTERS =========  */

        const glm::vec3 &get_position() const;

        void set_position(const glm::vec3 &position);

        const glm::vec3 &get_velocity() const;

        void set_velocity(const glm::vec3 &velocity);

        float get_mass() const;

        void set_mass(float mass);

        const glm::mat3 &get_rotation_matrix() const;

        void set_rotation_matrix(const glm::mat3 &rotation_matrix);

        const glm::mat3 &get_moment_of_inertia() const;

        void set_moment_of_inertia(const glm::mat3 &moment_of_inertia);

        float get_radius() const;

        void set_radius(float radius);

        Entity *get_parent() const;

        void set_parent(Entity *parent);

        const std::vector<Force> &get_applied_forces() const;

        const EntityStepOutput &get_last_eso() const;

        void set_last_eso(const EntityStepOutput &last_eso);
    };


}


#endif //COPTERSWARM_ENTITY_H
