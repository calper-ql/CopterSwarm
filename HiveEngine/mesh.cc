//
// Created by calpe on 6/22/2018.
//

#include "mesh.h"
#include <cmath>
#include <iostream>
#include "TriangleDescriptor.h"

namespace HiveEngine {

    Mesh::Mesh(unsigned expand_size) {
        this->expand_size = 1;
        expand_indices();

        if (expand_size != 0) {
            this->expand_size = expand_size;
        } else {
            this->expand_size = 1;
        }

    }

    Mesh::~Mesh() {

    }

    unsigned Mesh::get_next_indices(TriangleDescriptor *td) {
        if (available_indices.empty()) {
            expand_indices();
        }
        unsigned value = available_indices.front();
        triangle_descriptors[value] = td;
        available_indices.pop_front();
        return value;
    }

    void Mesh::expand_indices() {
        unsigned index = indices.size();
        for (unsigned i = 0; i < expand_size; ++i) {
            available_indices.push_back(index + i);
        }
        indices.resize(indices.size() + expand_size, glm::uvec3(0, 0, 0));
        states.resize(states.size() + expand_size, 0);
        triangle_descriptors.resize(triangle_descriptors.size() + expand_size, nullptr);
    }

    void Mesh::erase_indices(unsigned index) {
        if (index < indices.size()) {
            states[index] = 0;
            available_indices.push_back(index);
            release_attrib(indices[index][0]);
            release_attrib(indices[index][1]);
            release_attrib(indices[index][2]);
            indices[index][0] = 0;
            indices[index][1] = 0;
            indices[index][2] = 0;
            triangle_descriptors[index] = nullptr;
        }
    }

    unsigned Mesh::get_next_attrib() {
        if (available_attrib.empty()) {
            expand_attribs();
        }
        unsigned value = available_attrib.front();
        fresh_allocated_attribs.push_back(value);
        if (fresh_allocated_attribs.size() > vertices.size()) fresh_allocated_attribs.clear();
        usage_count[value] = 0;
        available_attrib.pop_front();
        return value;
    }

    void Mesh::expand_attribs() {
        unsigned index = vertices.size();
        for (unsigned i = 0; i < expand_size; ++i) {
            available_attrib.push_back(index + i);
        }
        vertices.resize(vertices.size() + expand_size, glm::vec3(0.0, 0.0, 0.0));
        colors.resize(colors.size() + expand_size, glm::vec4(0.0, 0.0, 0.0, 0.0));
        usage_count.resize(usage_count.size() + expand_size, 0);
    }

    void Mesh::release_attrib(unsigned index) {
        if (index < usage_count.size()) {
            if (usage_count[index] > 0) {
                usage_count[index]--;
                if (usage_count[index] == 0) {
                    available_attrib.push_back(index);
                }
            }
        }
    }

    void Mesh::use_attrib(unsigned index) {
        if (index < usage_count.size()) {
            usage_count[index]++;
        }
    }

    unsigned Mesh::get_max_lod() {
        unsigned lod = 0;
        for (size_t i = 0; i < triangle_descriptors.size(); ++i) {
            if (triangle_descriptors[i]) {
                auto l = triangle_descriptors[i]->lod_depth;
                if (lod < l) lod = l;
            }
        }
        return lod;
    }


}
