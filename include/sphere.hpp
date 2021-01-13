#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <algorithm>

#define DEFAULT_TEXU Vector3f(0,-1,0)
#define DEFAULT_TEXV Vector3f(1,0,0)

class Sphere : public Object3D {

public:

    Vector3f center;
    float radius;
    bool texture;
    Vector3f texU, texV;


    Sphere() {
        // unit ball at the center
        center.x() = 0;
        center.y() = 0;
        center.z() = 0;
        radius = 1;
    }

    Sphere(const Vector3f &c, float ra, Material *material, bool t = false) : Object3D(material), texture(t) {
        // ball with radius ra at position c
        center = c;
        radius = ra;
        if (t) {
            texU = DEFAULT_TEXU;
            texV = DEFAULT_TEXV;
        }
    }

    ~Sphere() override = default;

    Intersection intersect(const Ray &r, Hit &h, float tmin) override {
        // decide whether the ray intersects the ball
        // calculate vector OC and Ray direction(nomal)
        Vector3f OC = center - r.getOrigin();
        Vector3f dir = r.getDirection();
        dir.normalize();
        // calculate |CH|
        float loh = Vector3f::dot(dir, OC);
        float lch = sqrt(OC.length() * OC.length() - loh * loh);
        if (lch > radius)
            return MISS;
        else {
            // calculate t
            float lph = sqrt(radius * radius - lch * lch);
            float t = loh - lph;
            float t_ = loh + lph;
            if (t > h.getT() || (t < tmin && t_ > h.getT()))// not the smallest t among all tests
                return MISS;
            else {
                if (t_ < tmin) // make no sense
                    return MISS;
                else {
                    bool in = false;
                    if (t < tmin) {
                        in = true;
                        t = t_;
                    }
                    // calculate n at the intersection
                    Vector3f CP = r.getOrigin() + t * dir - center;
                    Vector3f n = CP.normalized();
                    //update
                    if (texture) {// add texture
                        float theta = acos(-Vector3f::dot(n, texV));
                        float phi = acos(std::min(std::max(Vector3f::dot(n, texU) / sin(theta), -1.f), 1.f));
                        float u = theta / M_PI, v = phi / (2 * M_PI);
                        v = (Vector3f::dot(n, Vector3f::cross(texU, texV)) < 0) ? (1 - v) : v;
                        h.texture = true;
                        h.texture_coordinate = Vector2f(u, v);
                    }
                    h.set(t, material, n, r.getOrigin() + t * dir, id);
                    return in ? INSIDE : OUTSIDE;
                }
            }
        }
    }

    // get the nearest distance and also the farther one
    float intersect_0(const Ray &r, float *tin = nullptr, float *tout = nullptr) const {

        // returns distance, 0 if nohit
        Vector3f OC = center - r.getOrigin();
        Vector3f dir = r.getDirection();
        dir.normalize();
        float t, eps = 1e-4;
        float b = Vector3f::dot(OC, dir);
        float det = b * b - Vector3f::dot(OC, OC) + radius * radius;
        if (det < 0)
            return 0;
        else
            det = sqrt(det);
        if (tin && tout) {
            *tin = (b - det <= 0) ? 0 : b - det;
            *tout = b + det;
        }
        return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
    }

    Vector4f get_light_info() override {
        return Vector4f(center.x(), center.y(), center.z(), radius);
    }
};


#endif

