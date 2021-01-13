#ifndef LIGHT_H
#define LIGHT_H


#include "vecmath.h"
#include "object3d.hpp"
#include "simple_method.hpp"

class Light {
public:
    Vector3f color;

    Light() {}

    virtual ~Light() {}

    virtual void getIllumination(const Vector3f &p, Vector3f &dir, Vector3f &col, float &distanceToLight) const = 0;

    virtual Vector3f get_random_point() const = 0;

};

class DirectionalLight : public Light {
public:

    DirectionalLight(const Vector3f &d, const Vector3f &c) {
        direction = d.normalized();
        color = c;
    }

    ~DirectionalLight() {

    }

    ///@param p unsed in this function
    ///@param distanceToLight not well defined because it's not a point light
    virtual void getIllumination(const Vector3f &p, Vector3f &dir, Vector3f &col, float &distanceToLight) const {
        // the direction to the light is the opposite of the
        // direction of the directional light source
        dir = -direction;
        col = color;
        distanceToLight = 1e38;
    }

    virtual Vector3f get_random_point() const {
        return -direction * 1e38;
    }

private:

    DirectionalLight(); // don't use

    Vector3f direction;

};

class PointLight : public Light {
public:

    PointLight(const Vector3f &p, const Vector3f &c, float fall) {
        position = p;
        color = c;
        falloff = fall;
    }

    ~PointLight() {

    }

    virtual void getIllumination(const Vector3f &p, Vector3f &dir, Vector3f &col, float &distanceToLight) const {
        // the direction to the light is the opposite of the
        // direction of the directional light source
        dir = (position - p);
        distanceToLight = dir.length();
        dir.normalize();
        col = color / (1 + falloff * distanceToLight * distanceToLight);
    }

    virtual Vector3f get_random_point() const {
        return position;
    }


private:

    PointLight(); // don't use
    float falloff;
    Vector3f position;

};

class SphereLight : public Light {
public:

    SphereLight(const Vector3f &p, const Vector3f &c, float r) {
        position = p;
        color = c;
        radius = r;
    }

    ~SphereLight() {

    }

    virtual void getIllumination(const Vector3f &p, Vector3f &dir, Vector3f &col, float &distanceToLight) const {
        // the direction to the light is the opposite of the
        // direction of the directional light source
        dir = (position - p);
        distanceToLight = dir.length();
        dir.normalize();
        col = color;
    }

    virtual Vector3f get_random_point() const {

        // get a random point on the sphere light
        Vector3f unit_random = random_sphere();
        unit_random *= radius; // gives a vector to its surface
        return position + unit_random;
    }


private:

    float radius;
    Vector3f position;

};

#endif // LIGHT_H