//
// Created by calper on 9/11/2018.
//

#ifndef HIVEENGINE_ENTITY_H
#define HIVEENGINE_ENTITY_H

#include <vector>
#include "../glm/glm.hpp"
#include "Utilities.h"

namespace HiveEngine {
    class Force {
    public:
        glm::vec3 leverage;
        glm::vec3 force;
        bool is_relative;

        Force(const glm::vec3 &leverage, const glm::vec3 &force, bool is_relative);
    };

    class Entity {
    private:
        glm::vec3 position; // expressed in meters
        glm::mat3 rotation_matrix; // expressed in radians
        float mass; // expressed in kilograms
        float radius; // expressed in meters

        glm::vec3 velocity; // expressed in meters per second
        glm::mat3 angular_velocity; // expressed in radians per second

        Entity* parent; // parent link
        std::vector<Entity*> children;

        std::vector<Force> applied_forces;

        float calculate_total_radius(glm::vec3 origin);

    public:
        Entity(glm::vec3 position, float radius, float mass);
        virtual ~Entity();

        float calculate_total_mass();
        float calculate_total_radius();

        float get_radius();
        void set_radius(float radius);

        glm::vec3 get_position();
        void set_position(glm::vec3 position);

        glm::mat3 calculate_rotation_matrix();
        glm::vec3 calculate_position();

        void apply_force(glm::vec3 leverage, glm::vec3 force, bool is_relative);
        std::vector<Force> get_applied_forces();

        virtual std::pair<glm::vec3, glm::vec3> step(); // pair -> (force, torque)
    };


}

#endif //DARKENGINE_ENTITY_H
