//
// Created by calper on 9/11/2018.
//

#include "Utilities.h"
#include "Entity.h"

namespace HiveEngine {

    glm::mat3 generate_rotation_matrix(char axis, float angle) {
        glm::mat3 mat(1.0);
        //mat[0][0] = 1.0f; mat[0][1] = 0.0f; mat[0][2] = 0.0f;
        //mat[1][0] = 0.0f; mat[1][1] = 1.0f; mat[1][2] = 0.0f;
        //mat[2][0] = 0.0f; mat[2][1] = 0.0f; mat[2][2] = 1.0f;


        float c = cos(angle);
        float s = sin(angle);

         if (axis == 'x' || axis =='X') {
            mat[1][1] = c;
            mat[2][1] = -s;
            mat[1][2] = s;
            mat[2][2] = c;
        } else if (axis == 'y' || axis =='Y'){
            mat[0][0] = c;
            mat[2][0] = s;
            mat[0][2] = -s;
            mat[2][2] = c;
        } else if (axis == 'z' || axis =='Z'){
            mat[0][0] = c;
            mat[0][1] = s;
            mat[1][0] = -s;
            mat[1][1] = c;
        }

        return mat;
    }

    std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>> generate_entity_line_description(Entity* e, glm::vec3 scale) {
        std::vector<glm::vec3> lines;
        std::vector<glm::vec3> colors;

        auto p = glm::vec3(0.0, 0.0, 0.0);

        // Makes the left vector green
        lines.emplace_back(p.x - e->get_radius()*scale.x, p.y, p.z);
        lines.emplace_back(p.x + e->get_radius()*scale.x, p.y, p.z);
        colors.emplace_back(0.0, 1.0, 0.0);
        colors.emplace_back(1.0, 1.0, 1.0);

        lines.emplace_back(p.x, p.y - e->get_radius()*scale.y, p.z);
        lines.emplace_back(p.x, p.y + e->get_radius()*scale.y, p.z);
        colors.emplace_back(1.0, 1.0, 1.0);
        colors.emplace_back(1.0, 1.0, 1.0);

        // Makes the up vector blue
        lines.emplace_back(p.x , p.y, p.z - e->get_radius()*scale.z);
        lines.emplace_back(p.x , p.y, p.z + e->get_radius()*scale.z);
        colors.emplace_back(1.0, 1.0, 1.0);
        colors.emplace_back(0.0, 0.0, 1.0);

        auto rm = e->calculate_rotation_matrix();
        for (size_t i = 0; i < lines.size(); ++i) {
            lines[i] = rm * lines[i];
            lines[i] = lines[i] + e->calculate_position();
        }

        return {lines, colors};
    }

    std::string vec3_to_str(glm::vec3 value) {
        std::string str;
        str.append("(");
        str.append(std::to_string(value.x));
        str.append(", ");
        str.append(std::to_string(value.y));
        str.append(", ");
        str.append(std::to_string(value.z));
        str.append(")");
        return str;
    }
    
}