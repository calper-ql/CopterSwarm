//
// Created by calpe on 6/22/2018.
//

#ifndef HIVEENGINE_MESH_H
#define HIVEENGINE_MESH_H

#include <cstddef>
#include "../glm/glm.hpp"
#include <vector>
#include <queue>

namespace HiveEngine {

    class TriangleDescriptor;

    class Mesh {
    public:

        unsigned expand_size;

        std::deque<unsigned> available_attrib;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec4> colors;
        std::vector<unsigned> usage_count;

        std::deque<unsigned> available_indices;
        std::vector<glm::uvec3> indices;
        std::vector<TriangleDescriptor *> triangle_descriptors;
        std::vector<char> states;

        std::vector<unsigned> fresh_allocated_attribs;

        Mesh(unsigned expand_size);

        ~Mesh();

        unsigned get_next_attrib();

        void expand_attribs();

        void release_attrib(unsigned index);

        void use_attrib(unsigned index);

        unsigned get_next_indices(TriangleDescriptor *td);

        void expand_indices();

        void erase_indices(unsigned index);

        unsigned get_max_lod();

    };

};

#endif //HIVEENGINE_MESH_H
