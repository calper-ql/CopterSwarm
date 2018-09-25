//
// Created by calper on 9/11/2018.
//

#include "Utilities.h"
#include "Entity.h"
#include "TriangleDescriptor.h"


namespace HiveEngine {

    glm::mat3 generate_rotation_matrix(char axis, float angle) {
        glm::mat3 mat(1.0);
        //mat[0][0] = 1.0f; mat[0][1] = 0.0f; mat[0][2] = 0.0f;
        //mat[1][0] = 0.0f; mat[1][1] = 1.0f; mat[1][2] = 0.0f;
        //mat[2][0] = 0.0f; mat[2][1] = 0.0f; mat[2][2] = 1.0f;

        float c = cosf(angle);
        float s = sinf(angle);

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

    std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>> generate_target_line_description(glm::vec3 vec, float radius, glm::vec3 scale , glm::vec3 color){
        std::vector<glm::vec3> lines;
        std::vector<glm::vec3> colors;

        auto p = glm::vec3(0.0, 0.0, 0.0);

        // Makes the left vector green
        lines.emplace_back(p.x - radius*scale.x, p.y, p.z);
        lines.emplace_back(p.x + radius*scale.x, p.y, p.z);
        colors.push_back(color);
        colors.push_back(color);

        lines.emplace_back(p.x, p.y - radius*scale.y, p.z);
        lines.emplace_back(p.x, p.y + radius*scale.y, p.z);
        colors.push_back(color);
        colors.push_back(color);

        // Makes the up vector blue
        lines.emplace_back(p.x , p.y, p.z - radius*scale.z);
        lines.emplace_back(p.x , p.y, p.z + radius*scale.z);
        colors.push_back(color);
        colors.push_back(color);

        for (size_t i = 0; i < lines.size(); ++i) {
            lines[i] = lines[i];
            lines[i] = lines[i] + vec;
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

    void add_voxel_to_mesh(Mesh *m, glm::vec3 pos, float size, glm::vec4 color) {
        auto luc = m->get_next_attrib();
        auto luf = m->get_next_attrib();
        auto ldc = m->get_next_attrib();
        auto ldf = m->get_next_attrib();

        auto ruc = m->get_next_attrib();
        auto ruf = m->get_next_attrib();
        auto rdc = m->get_next_attrib();
        auto rdf = m->get_next_attrib();

        m->vertices[luc] = glm::vec3(pos.x-size, pos.y+size, pos.z-size);
        m->vertices[luf] = glm::vec3(pos.x-size, pos.y+size, pos.z+size);
        m->vertices[ldc] = glm::vec3(pos.x-size, pos.y-size, pos.z-size);
        m->vertices[ldf] = glm::vec3(pos.x-size, pos.y-size, pos.z+size);

        m->vertices[ruc] = glm::vec3(pos.x+size, pos.y+size, pos.z-size);
        m->vertices[ruf] = glm::vec3(pos.x+size, pos.y+size, pos.z+size);
        m->vertices[rdc] = glm::vec3(pos.x+size, pos.y-size, pos.z-size);
        m->vertices[rdf] = glm::vec3(pos.x+size, pos.y-size, pos.z+size);

        m->colors[luc] = color;
        m->colors[luf] = color;
        m->colors[ldc] = color;
        m->colors[ldf] = color;

        m->colors[ruc] = color;
        m->colors[ruf] = color;
        m->colors[rdc] = color;
        m->colors[rdf] = color;

        auto left_1 = new HiveEngine::TriangleDescriptor(m, TI(ldf), TI(luf), TI(luc), 0, NO_PARENT);
        auto left_2 = new HiveEngine::TriangleDescriptor(m, TI(luc), TI(ldc), TI(ldf), 0, NO_PARENT);
        left_1->set_neighbor(left_2, LR);

        auto up_1 = new HiveEngine::TriangleDescriptor(m, TI(luf), TI(ruf), TI(ruc), 0, NO_PARENT);
        auto up_2 = new HiveEngine::TriangleDescriptor(m, TI(ruc), TI(luc), TI(luf), 0, NO_PARENT);
        up_1->set_neighbor(up_2, LR);

        auto down_1 = new HiveEngine::TriangleDescriptor(m, TI(ldf), TI(rdf), TI(rdc), 0, NO_PARENT);
        auto down_2 = new HiveEngine::TriangleDescriptor(m, TI(rdc), TI(ldc), TI(ldf), 0, NO_PARENT);
        down_1->set_neighbor(down_2, LR);

        auto right_1 = new HiveEngine::TriangleDescriptor(m, TI(rdf), TI(ruf), TI(ruc), 0, NO_PARENT);
        auto right_2 = new HiveEngine::TriangleDescriptor(m, TI(ruc), TI(rdc), TI(rdf), 0, NO_PARENT);
        right_1->set_neighbor(right_2, LR);

        auto close_1 = new HiveEngine::TriangleDescriptor(m, TI(ldc), TI(luc), TI(ruc), 0, NO_PARENT);
        auto close_2 = new HiveEngine::TriangleDescriptor(m, TI(ruc), TI(rdc), TI(ldc), 0, NO_PARENT);
        close_1->set_neighbor(close_2, LR);

        auto far_1 = new HiveEngine::TriangleDescriptor(m, TI(ldf), TI(luf), TI(ruf), 0, NO_PARENT);
        auto far_2 = new HiveEngine::TriangleDescriptor(m, TI(ruf), TI(rdf), TI(ldf), 0, NO_PARENT);
        far_1->set_neighbor(far_2, LR);

    }

}