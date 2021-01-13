#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "ray.hpp"
#include "hit.hpp"
#include "material.hpp"

enum Intersection
{
    OUTSIDE = 1, 	// from outside
    INSIDE = -1,    // from inside(Normal)
    MISS = 0
};

// Base class for all 3d entities.
class Object3D {
public:

    Material *material;
    int id;

    Object3D() : material(nullptr) {}

    virtual ~Object3D() = default;

    explicit Object3D(Material *material) {
        this->material = material;
        id=-1;
    }

    // Intersect Ray with this object. If hit, store information in hit structure.
    virtual Intersection intersect(const Ray &r, Hit &h, float tmin) = 0;

    // Get infomation for position and radius for sphere light object
    virtual Vector4f get_light_info() { return Vector4f(0, 0, 0, 0); }

};

#endif

