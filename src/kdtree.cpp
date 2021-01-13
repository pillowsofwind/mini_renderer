#include "kdtree.hpp"
#include <climits>
#include <iostream>
#include <algorithm>

using namespace std;

void KDTree::load(int _size, HitPoint *_data) {
    m_nNode = 0;
    m_size = _size;
    m_data = _data;
    m_index = new int[m_size];
    m_memory = new Node[m_size];
    for (int i = 0; i < m_size; ++i)
        m_index[i] = i;
}

void KDTree::medianPartition(int l, int r, int dim, int k) {
    int mid = (l + r) >> 1, temp = m_index[mid];
    int i = l, j = r;
    while (i < j) {
        while (m_data[m_index[i]].position[dim] < m_data[temp].position[dim]) ++i;
        while (m_data[m_index[j]].position[dim] > m_data[temp].position[dim]) --j;
        if (i <= j) {
            int t = m_index[i];
            m_index[i] = m_index[j];
            m_index[j] = t;
            ++i;
            --j;
        }
    }
    if (l < j && l <= k && k <= j) medianPartition(l, j, dim, k);
    if (i < r && i <= k && k <= r) medianPartition(i, r, dim, k);
}

KDTree::Node *KDTree::build(int l, int r, double *min, double *max) {
    if (r <= l) return nullptr;
    float temp = -1;
    int split;
    for (int i = 0; i < K; ++i)
        if (max[i] - min[i] > temp) {
            temp = max[i] - min[i];
            split = i;
        }
    int mid = (l + r) >> 1;
    medianPartition(l, r - 1, split, mid);
    m_memory[m_nNode].value = m_index[mid];
    m_memory[m_nNode].split = split;
    Node *node = &m_memory[m_nNode++];

    temp = max[split];
    max[split] = m_data[m_index[mid]].position[split];
    node->left = build(l, mid, min, max);
    max[split] = temp;

    temp = min[split];
    min[split] = m_data[m_index[mid]].position[split];
    node->right = build(mid + 1, r, min, max);
    min[split] = temp;

    m_data[node->value].maxRadius2 = m_data[node->value].radius2;
    if (node->left && m_data[node->left->value].maxRadius2 > m_data[node->value].maxRadius2)
        m_data[node->value].maxRadius2 = m_data[node->left->value].maxRadius2;

    if (node->right && m_data[node->right->value].maxRadius2 > m_data[node->value].maxRadius2)
        m_data[node->value].maxRadius2 = m_data[node->right->value].maxRadius2;

    return node;
}

void KDTree::build() {
    m_nNode = 0;
    double *min = new double[K], *max = new double[K];
    for (int i = 0; i < K; ++i) min[i] = LONG_MAX, max[i] = LONG_MIN;
    for (int i = 0; i < m_size; ++i) {
        Vector3f temp = m_data[i].position;;
        for (int j = 0; j < K; ++j) {
            if (m_data[i].position[j] < min[j]) min[j] = m_data[i].position[j];
            if (m_data[i].position[j] > max[j]) max[j] = m_data[i].position[j];
        }
    }
    m_root = build(0, m_size, min, max);
}

void KDTree::insertPhoton(Node *node, const Photon &photon) {
    if (node == NULL) return;

    int pos = node->value;
    if (Vector3f::dot(m_data[pos].position - photon.P, m_data[pos].position - photon.P) < m_data[pos].radius2)
        if (photon.object == m_data[pos].object) {
            m_data[pos].nNew += 1;
            m_data[pos].phi += photon.color;
        }

    int split = node->split;
    Node *another;
    if (photon.P[split] < m_data[pos].position[split]) {
        another = node->right;
        insertPhoton(node->left, photon);
    } else {
        another = node->left;
        insertPhoton(node->right, photon);
    }
    if ((another) &&
        (m_data[pos].position[split] - photon.P[split]) * (m_data[pos].position[split] - photon.P[split]) <
        m_data[another->value].maxRadius2 + 1e6)
        insertPhoton(another, photon);
}

void KDTree::insertPhoton(const Photon &photon) {
    insertPhoton(m_root, photon);
}

void KDTree::update(Node *node) {
    if (node->left) update(node->left);
    if (node->right) update(node->right);
    m_data[node->value].maxRadius2 = m_data[node->value].radius2;
    if (node->left && m_data[node->left->value].maxRadius2 > m_data[node->value].maxRadius2)
        m_data[node->value].maxRadius2 = m_data[node->left->value].maxRadius2;

    if (node->right && m_data[node->right->value].maxRadius2 > m_data[node->value].maxRadius2)
        m_data[node->value].maxRadius2 = m_data[node->right->value].maxRadius2;
}

void KDTree::update() {
    update(m_root);
}

KDTree::~KDTree() {
//    if(m_root) delete m_root;
//    if(m_index) delete m_index;
//    if(m_memory)delete m_memory;
}