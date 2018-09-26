//
// Created by calpe on 8/26/2018.
//

#include "TriangleDescriptor.h"
#include "mesh.h"


namespace HiveEngine {

    glm::vec3 __hiveengine_mesh_get_midpoint(glm::vec3 a, glm::vec3 b) {
        return b + ((a - b) / glm::vec3(2.0, 2.0, 2.0));
    }

    TriangleInstruction::TriangleInstruction(unsigned index) {
        this->allocated = true;
        this->index = index;
    }

    TriangleInstruction::TriangleInstruction(glm::vec3 vec) {
        this->allocated = false;
        this->vec = vec;
    }

    TriangleDescriptor::TriangleDescriptor(Mesh *mesh, TI left, TI up, TI right, unsigned lod_depth,
                                           TDChildState child_state) {
        this->mesh = mesh;
        this->index = mesh->get_next_indices(this);
        this->lod_depth = lod_depth;
        this->child_state = child_state;

        ul_neighbor = nullptr;
        ur_neighbor = nullptr;
        lr_neighbor = nullptr;

        left_child = nullptr;
        right_child = nullptr;

        parent = nullptr;
        square = nullptr;
        should_erase = false;

        apply_instruction(left, LEFT);
        apply_instruction(up, UP);
        apply_instruction(right, RIGHT);

        disable();
        enable();
    }

    TriangleDescriptor::~TriangleDescriptor() {
        erase_subdiv();
        mesh->erase_indices(index);
        index = 0;
    }

    void TriangleDescriptor::apply_instruction(TI inst, short loc) {
        if (inst.allocated) {
            mesh->use_attrib(inst.index);
            mesh->indices[index][loc] = inst.index;
        } else {
            unsigned v_index = mesh->get_next_attrib();
            mesh->vertices[v_index] = inst.vec;
            mesh->indices[index][loc] = v_index;
            mesh->use_attrib(v_index);
        }
    }

    glm::vec3 TriangleDescriptor::get_point(TriangleDescriptorSide tds) {
        return mesh->vertices[mesh->indices[index][tds]];
    }

    void TriangleDescriptor::set_point(TriangleDescriptorSide tds, glm::vec3 vec) {
        mesh->vertices[mesh->indices[index][tds]] = vec;
    }


    unsigned TriangleDescriptor::get_side_index(TriangleDescriptorSide tds) {
        return mesh->indices[index][tds];
    }

    void TriangleDescriptor::set_side_index(TriangleDescriptorSide tds, unsigned idx) {
        mesh->indices[index][tds] = idx;
    }

    void TriangleDescriptor::disable() {
        local[UP] = get_side_index(UP);
        local[LEFT] = get_side_index(LEFT);
        local[RIGHT] = get_side_index(RIGHT);
        set_side_index(UP, 0);
        set_side_index(LEFT, 0);
        set_side_index(RIGHT, 0);
    }

    void TriangleDescriptor::enable() {
        set_side_index(UP, local[UP]);
        set_side_index(LEFT, local[LEFT]);
        set_side_index(RIGHT, local[RIGHT]);
    }

    void TriangleDescriptor::subdiv(unsigned depth) {
        std::vector<TriangleDescriptor *> tbsd;
        std::vector<TriangleDescriptor *> temp;
        tbsd.push_back(this);
        for (unsigned i = 0; i < depth; i++) {
            for (size_t k = 0; k < tbsd.size(); k++) {

                tbsd[k]->subdiv_internal();
                temp.push_back(tbsd[k]->left_child);
                temp.push_back(tbsd[k]->right_child);

            }
            tbsd = temp;
            temp.clear();
        }

    }

    void TriangleDescriptor::subdiv_internal() {
        if (has_children()) return;
        glm::vec3 rl_mid = __hiveengine_mesh_get_midpoint(get_point(LEFT), get_point(RIGHT));
        unsigned rmi = mesh->get_next_attrib();
        mesh->vertices[rmi] = rl_mid;
        subdiv_with_common(rmi);
    }

    TriangleDescriptor *TriangleDescriptor::check_lr_neighbor() {
        if (lr_neighbor) {
            if (lod_depth + 1 - lr_neighbor->lod_depth >= 2) {
                lr_neighbor->subdiv_internal();
                if (child_state == LEFT_CHILD) {
                    return lr_neighbor->right_child;
                } else if (child_state == RIGHT_CHILD) {
                    return lr_neighbor->left_child;
                }
            }
        }
        return lr_neighbor;
    }

    float r_f(float min, float max) {
        return min + static_cast<float>(rand()) / (RAND_MAX / (max - min));
    }

    void TriangleDescriptor::subdiv_with_common(unsigned rmi) {
        if (has_children()) return;

        auto temp_lr = check_lr_neighbor();
        //lr_neighbor = temp_lr;

        left_child = new TriangleDescriptor(mesh, TI(get_side_index(UP)), TI(rmi), TI(get_side_index(LEFT)),
                                            lod_depth + 1, LEFT_CHILD);
        right_child = new TriangleDescriptor(mesh, TI(get_side_index(RIGHT)), TI(rmi), TI(get_side_index(UP)),
                                             lod_depth + 1, RIGHT_CHILD);

        left_child->ul_neighbor = right_child;
        right_child->ur_neighbor = left_child;

        right_child->lr_neighbor = ur_neighbor;
        left_child->lr_neighbor = ul_neighbor;

        if (temp_lr) {
            //temp_lr->subdiv_with_common(rmi);


            temp_lr->left_child = new TriangleDescriptor(mesh, TI(temp_lr->get_side_index(UP)), TI(rmi),
                                                         TI(temp_lr->get_side_index(LEFT)),
                                                         lod_depth + 1, LEFT_CHILD);
            temp_lr->right_child = new TriangleDescriptor(mesh, TI(temp_lr->get_side_index(RIGHT)), TI(rmi),
                                                          TI(temp_lr->get_side_index(UP)),
                                                          lod_depth + 1, RIGHT_CHILD);

            temp_lr->left_child->ul_neighbor = temp_lr->right_child;
            temp_lr->right_child->ur_neighbor = temp_lr->left_child;

            temp_lr->right_child->lr_neighbor = temp_lr->ur_neighbor;
            temp_lr->left_child->lr_neighbor = temp_lr->ul_neighbor;

            temp_lr->left_child->ur_neighbor = right_child;
            temp_lr->right_child->ul_neighbor = left_child;
            right_child->ul_neighbor = temp_lr->left_child;
            left_child->ur_neighbor = temp_lr->right_child;

            TDSquare *sq = new TDSquare();
            sq->a = left_child;
            sq->b = right_child;
            sq->c = temp_lr->left_child;
            sq->d = temp_lr->right_child;
            sq->ab_p = this;
            sq->cd_p = temp_lr;

            left_child->square = sq;
            right_child->square = sq;
            temp_lr->left_child->square = sq;
            temp_lr->right_child->square = sq;

            temp_lr->left_child->parent = temp_lr;
            temp_lr->right_child->parent = temp_lr;

            temp_lr->disable();

        } else {
            right_child->ul_neighbor = nullptr;
            left_child->ur_neighbor = nullptr;
        }

        left_child->parent = this;
        right_child->parent = this;

        disable();
    }

    bool TriangleDescriptor::has_children() {
        return left_child != nullptr && right_child != nullptr;
    }

    void TriangleDescriptor::erase_subdiv() {
        if (!has_children())
            return;
        if (lod_depth <= 10) return;

        if (left_child->square) {
            left_child->should_erase = true;
            right_child->should_erase = true;
            auto sq = left_child->square;
            if (true) {
                delete sq->a;
                delete sq->b;
                delete sq->c;
                delete sq->d;
                sq->ab_p->left_child = nullptr;
                sq->ab_p->right_child = nullptr;
                sq->cd_p->left_child = nullptr;
                sq->cd_p->right_child = nullptr;
                sq->cd_p->enable();
                sq->ab_p->enable();
                delete sq;
            } else {
                return;
            }
        }

        delete left_child;
        delete right_child;
        left_child = nullptr;
        right_child = nullptr;
        enable();
    }

    TDNeighborState TriangleDescriptor::get_neighbor_standing(TDNeighborState ns) {
        TriangleDescriptor *n;
        if (ns == None) return None;
        else if (ns == UL) n = this->ul_neighbor;
        else if (ns == UR) n = this->ur_neighbor;
        else if (ns == LR) n = this->lr_neighbor;
        if (n == nullptr) return None;
        if (this == n->ul_neighbor) return UL;
        else if (this == n->ur_neighbor) return UR;
        else if (this == n->lr_neighbor) return LR;
        else return None;
    }

    void TriangleDescriptor::set_neighbor(TriangleDescriptor *n, TDNeighborState ns) {
        if (ns == UL) ul_neighbor = n;
        else if (ns == UR) ur_neighbor = n;
        else if (ns == LR) lr_neighbor = n;
    }

    bool TriangleDescriptor::operator()(const TriangleDescriptor *lhs, const TriangleDescriptor *rhs) {
        return lhs->lod_depth < rhs->lod_depth;
    }

}

