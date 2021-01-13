#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement Plane representing an infinite plane
// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions

class Plane : public Object3D {
public:

    bool texture;
    Vector3f texU, texV;

    Plane() {

    }

    Plane(const Vector3f &n, float f, Material *m, bool t = false) : Object3D(m), texture(t) {
        normal = n;
        d = f;
    }

    ~Plane() override = default;

    Intersection intersect(const Ray &r, Hit &h, float tmin) override {
        // calculate t
        Vector3f O = r.getOrigin();
        Vector3f dir = r.getDirection();
        float t = (d - Vector3f::dot(normal, O)) / Vector3f::dot(normal, dir);
        if (t > h.getT())// not the smallest among all the tests
            return MISS;
        else {
            if (t < tmin)// make no sense
                return MISS;
            else {
                // update
                Vector3f P = r.pointAtParameter(t);
                h.set(t, material, normal, P, id);
                return OUTSIDE;
            }
        }
    }

protected:
    Vector3f normal;
    float d;
};

#endif //PLANE_H
		

