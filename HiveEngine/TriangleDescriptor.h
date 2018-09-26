//
// Created by calpe on 8/26/2018.
//

#ifndef HIVEENGINE_TRIANGLEDESCRIPTOR_H
#define HIVEENGINE_TRIANGLEDESCRIPTOR_H

#include <cstddef>
#include "../glm/glm.hpp"
#include <vector>

namespace HiveEngine {
    class Mesh;

    class TriangleDescriptor;

    class TriangleInstruction {
    public:
        bool allocated;
        unsigned index;
        glm::vec3 vec;

        TriangleInstruction(unsigned index);

        TriangleInstruction(glm::vec3 vec);
    };

    typedef TriangleInstruction TI;

    enum TriangleDescriptorSide {
        LEFT = 0,
        UP = 1,
        RIGHT = 2,
    };

    enum TDChildState {
        LEFT_CHILD = 0,
        NO_PARENT = 1,
        RIGHT_CHILD = 2,
    };

    enum TDNeighborState {
        UL = 0,
        UR = 1,
        LR = 2,
        None = 3,
    };

    struct TDSquare {
        TriangleDescriptor *a;
        TriangleDescriptor *b;
        TriangleDescriptor *c;
        TriangleDescriptor *d;
        TriangleDescriptor *ab_p;
        TriangleDescriptor *cd_p;
    };

    class TriangleDescriptor {

    public:
        unsigned index;
        Mesh *mesh;
        glm::uvec3 local;
        unsigned lod_depth;

        TriangleDescriptor *ul_neighbor;
        TriangleDescriptor *ur_neighbor;
        TriangleDescriptor *lr_neighbor;
        TDSquare *square;

        TDChildState child_state;
        TriangleDescriptor *parent;
        TriangleDescriptor *left_child;
        TriangleDescriptor *right_child;
        bool should_erase;

        TriangleDescriptor(Mesh *mesh, TI left, TI up, TI right, unsigned lod_depth, TDChildState child_state);

        ~TriangleDescriptor();

        void apply_instruction(TI inst, short loc);

        glm::vec3 get_point(TriangleDescriptorSide tds);

        unsigned get_side_index(TriangleDescriptorSide tds);

        void set_side_index(TriangleDescriptorSide tds, unsigned idx);

        void set_point(TriangleDescriptorSide tds, glm::vec3 vec);

        void disable();

        void enable();

        bool has_children();

        void subdiv(unsigned depth);

        void subdiv_internal();

        TriangleDescriptor *check_lr_neighbor();

        void subdiv_with_common(unsigned rmi);

        void erase_subdiv();

        TDNeighborState get_neighbor_standing(TDNeighborState ns);

        void set_neighbor(TriangleDescriptor *n, TDNeighborState ns);

        bool operator()(const TriangleDescriptor *lhs, const TriangleDescriptor *rhs);

    };
}


#endif //HIVEENGINE_TRIANGLEDESCRIPTOR_H
