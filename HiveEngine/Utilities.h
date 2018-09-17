//
// Created by calper on 9/11/2018.
//

#ifndef HIVEENGINE_UTILITIES_H
#define HIVEENGINE_UTILITIES_H

#include "../glm/glm.hpp"
#include <cmath>
#include <vector>
#include <string>

namespace HiveEngine {
    class Entity;
    glm::mat3 generate_rotation_matrix(char axis, float angle); // angle is radians
    std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>> generate_entity_line_description(Entity* e, glm::vec3 scale); // generates lines for renderer, scales each dim, 0.0 -> 1.0
    std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>> generate_target_line_description(glm::vec3 vec, float radius, glm::vec3 scale, glm::vec3 color);
    std::string vec3_to_str(glm::vec3 value);
}

#endif //DARKENGINE_UTILITIES_H
