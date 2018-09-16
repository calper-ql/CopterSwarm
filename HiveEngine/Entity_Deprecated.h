//
// Created by calper on 9/11/2018.
//

#ifndef HIVEENGINE_DEP_ENTITY_H
#define HIVEENGINE_DEP_ENTITY_H

#include <vector>
#include "../glm/glm.hpp"
#include "Utilities.h"
#include "../glm/gtc/quaternion.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "CommonStructs.h"

namespace HiveEngine {


    class Entity_Deprecated {
    private:
        glm::vec3 position; // expressed in meters
        glm::mat3 rotation_matrix; // expressed in radians
        float mass; // expressed in kilograms
        float radius; // expressed in meters

        glm::vec3 velocity; // expressed in meters per second
        glm::quat angular_velocity; // expressed in radians per second
        glm::vec3 angular_acceleration_counter;

        glm::mat3 moment_of_inertia;
        glm::mat3 calculated_moi;

        Entity_Deprecated* parent; // parent link
        std::vector<Entity_Deprecated*> children;

        std::vector<Force> applied_forces;

        float calculate_total_radius(glm::vec3 origin);

    public:
        Entity_Deprecated(glm::vec3 position, float radius, float mass);
        virtual ~Entity_Deprecated();

        float calculate_total_mass();
        float calculate_total_radius();

        float get_radius();
        void set_radius(float radius);

        glm::vec3 get_position();
        void set_position(glm::vec3 position);

        void add_child(Entity_Deprecated* c);
        std::vector<Entity_Deprecated*> get_children();

        glm::vec3 get_velocity();
        void set_velocity(glm::vec3 velocity);

        void set_mass(float mass);
        float get_mass();

        void set_rotation_matrix(glm::mat3 rm);

        glm::mat3 get_moment_of_inertia();

        glm::mat3 get_rotation_matrix();
        glm::quat get_angular_velocity();
        glm::vec3 get_total_angular_acceleration();
        glm::vec3 calculate_throw_acceleration(glm::vec3 relative_point, bool parent_supported);

        glm::mat3 calculate_rotation_matrix();
        glm::vec3 calculate_position();

        virtual void apply_force(glm::vec3 leverage, glm::vec3 force, bool is_relative);
        std::vector<Force> get_applied_forces();

        virtual EntityStepOutput step(unsigned steps_per_second); // pair -> (force, torque)
    };


}

#endif //DARKENGINE_ENTITY_H
