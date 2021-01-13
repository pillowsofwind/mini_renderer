//
// Created by XRW's notebook on 2020/6/16.
//

#ifndef RAY_TRACER_KDTREE_HPP
#define RAY_TRACER_KDTREE_HPP

#include "Vector3f.h"
#include "object3d.hpp"

#include <algorithm>
#include <climits>

class Photon {
public:

    Vector3f origin, direction;
    Vector3f P; // place it hit
    Vector3f color;
    Object3D *object;

    Photon(const Vector3f &o, const Vector3f &d, const Vector3f &c) : origin(o), direction(d), color(c) {}
};


class KDTree {

    // a three dimension tree (K=3)
    static const int K = 3;
    struct Node {
        int value;
        int split;
        Node *left, *right;
    } *m_memory;


private:
    Node *m_root;
    int m_size; // numbers of nodes
    int *m_index;
    int m_nNode;
    HitPoint *m_data;

    void medianPartition(int l, int r, int dim, int k); // get median
    Node *build(int l, int r, double *min, double *max);

    void insertPhoton(Node *node, const Photon &photon);

    void update(Node *node);

public:
    ~KDTree();

    HitPoint *data(int &size) const {
        size = m_size;
        return m_data;
    }

    void build();

    void load(int size, HitPoint *data);

    void insertPhoton(const Photon &photon);

    void update();

    void init(int size, HitPoint *data) {
        load(size, data);
        build();
    }

};

#endif //RAY_TRACER_KDTREE_HPP
