#ifndef HIT_H
#define HIT_H

#include <vecmath.h>
#include "ray.hpp"
#include "object3d.hpp"

class Material;

class Object3D;

class Hit {
public:
    float t;
    Material *material;
    Vector3f normal;
    Vector3f position;
    bool texture;
    Vector2f texture_coordinate;
    Object3D *object;
    int id; // id related to hit object

    // constructors
    Hit() {
        material = nullptr;
        t = 1e38;
        texture = false;
        object = nullptr;
        id = -1;
    }

    Hit(float _t, Material *m, const Vector3f &n, int i) {
        t = _t;
        material = m;
        normal = n;
        texture = false;
        object = nullptr;
        id = i;
    }

    Hit(const Hit &h) {
        t = h.t;
        material = h.material;
        normal = h.normal;
        position = h.position;
        texture = h.texture;
        texture_coordinate = h.texture_coordinate;
        object = h.object;
        id = h.id;
    }

    // destructor
    ~Hit(){};

    float getT() const {
        return t;
    }

    Material *getMaterial() const {
        return material;
    }

    const Vector3f &getNormal() const {
        return normal;
    }

    void set(float _t, Material *m, const Vector3f &n, const Vector3f &p, int i) {
        t = _t;
        material = m;
        normal = n;
        position = p;
        id = i;
    }

};

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}

class HitPoint : public Hit {
public:

    // some other features, mainly according to Henrik's paper
    int row, col;    // pixel location
    Vector3f weight;    // pixel weight
    Vector3f phi;   // accumulative reflected flux
    float radius2, maxRadius2;  //  current radius , max radius from the tree
    float nAccum, nNew;    // accumulative count , new from last collision

    HitPoint() {}

    HitPoint(int r, int c, const Vector3f &w) : row(r), col(c), weight(w), phi(Vector3f::ZERO), nAccum(0), nNew(0) {}

    void update(float alpha) { // radius reduction
        if (nAccum <= 0 || nNew <= 0)
            return;
        float k = (nAccum + alpha * nNew) / (nAccum + nNew);
        radius2 *= k; // reduce
        phi *= k;
        nAccum += alpha * nNew; // update
        nNew = 0;
    }
};

#endif // HIT_H
