//
// Created by XRW's notebook on 2020/6/13.
//

#ifndef RAY_TRACER_SIMPLE_METHOD_HPP
#define RAY_TRACER_SIMPLE_METHOD_HPP

#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdio>
#include <algorithm>

inline double erand48(unsigned short xsubi[3]) {

    // genarate a random number in [0,1]
    return (double) rand() / (double) RAND_MAX;
}


inline Vector3f reflect(const Vector3f &normal, const Vector3f &in) {

    // calculate reflection ray direction
    return (in - normal * 2 * Vector3f::dot(in, normal)).normalized();
}


inline bool refract(const Vector3f &normal, const Vector3f &in, float n, float nt, Vector3f &refracted) {

    // with refract result returning
    // no refraction, no change direction(though reflected)
    Vector3f d = in.normalized();
    float dt = Vector3f::dot(d, normal);
    float ni = n / nt;
    float discriminant = 1.0 - ni * ni * (1 - dt * dt);
    if (discriminant > 0) {
        refracted = (ni * (d - normal * dt) - normal * sqrt(discriminant)).normalized();
        return true;
    } else
        return false;
}


//inline Vector3f refract(const Vector3f &normal, const Vector3f &in, float n) {
//
//    float cosI = -Vector3f::dot(in, normal);
//    float cosR2 = 1 - (1 - pow(cosI, 2)) * pow(n, 2);
//    return (cosR2 > 1e-6) ? // reflect?
//           in * n + normal * (n * cosI - sqrt(cosR2))
//                          : reflect(normal, in);
//}


inline float schlick_fresnel(float cosine, float nt, float n) {

    // an approximation for fresnal
    float r0 = (nt - n) / (nt + n);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5.0);
}



inline float sample_segment(float eps, float sigma, float s_max) {
    return -log(1.0 - eps * (1.0 - exp(-sigma * s_max))) / sigma;
}


inline Vector3f sample_sphere(float e1, float e2) {
    float z = 1.0 - 2.0 * e1, sint = sqrt(1.0 - z * z);
    return Vector3f(cos(2.0 * M_PI * e2) * sint, sin(2.0 * M_PI * e2) * sint, z);
}


inline Vector3f sampleHG(float g, float e1, float e2) {
    float s = 1.0 - 2.0 * e1;
    float cost = (s + 2.f * g * g * g * (-1.0 + e1) * e1 + g * g * s + 2.f * g * (1.0 - e1 + e1 * e1)) /
                 ((1.0 + g * s) * (1.0 + g * s));
    float sint = sqrt(1.0 - cost * cost);
    return Vector3f(cos(2.0 * M_PI * e2) * sint, sin(2.0 * M_PI * e2) * sint, cost);
}


inline void generate_cartesian(Vector3f &u, Vector3f &v, Vector3f w) {

    // build cartesian with w(normal)
    Vector3f coVec = w;
    if (fabs(w.x()) <= fabs(w.y()))
        if (fabs(w.x()) <= fabs(w.z())) coVec = Vector3f(0, -w.z(), w.y());
        else coVec = Vector3f(-w.y(), w.x(), 0);
    else if (fabs(w.y()) <= fabs(w.z())) coVec = Vector3f(-w.z(), 0, w.x());
    else coVec = Vector3f(-w.y(), w.x(), 0);
    coVec.normalize();
    u = Vector3f::cross(w, coVec), v = Vector3f::cross(w, u);
}


inline float scatter(const Ray &r, Ray *scattered, float tin, float tout, float &s, float sigma_s) {

    // scatter effect
    unsigned short Xi[3] = {static_cast<unsigned short>(rand() % 10000), static_cast<unsigned short>(rand() % 1000), 0};
    s = sample_segment(erand48(Xi), sigma_s, tout - tin);
    Vector3f x = r.pointAtParameter(tin) + r.getDirection() * s;

    Vector3f dir = sampleHG(-0.5, erand48(Xi), erand48(Xi)); // sample a direction ~ Henyey-Greenstein's phase function
    Vector3f u, v;
    generate_cartesian(u, v, r.getDirection());
    dir = u * dir.x() + v * dir.y() + r.getDirection() * dir.z();
    if (scattered) *scattered = Ray(x, dir);
    return (1.0 - exp(-sigma_s * (tout - tin)));
}


inline float cal_power(const Vector3f &color) {
    return (color.x() + color.y() + color.z()) / 3.f;
}


inline Vector3f random_hemi_sphere(const Vector3f &normal) {

    // first, build cartesian coordinate with normal
    Vector3f w = normal.normalized();
    Vector3f u = (Vector3f::cross((fabs(w.x()) > .1 ? Vector3f(0, 1, 0) : Vector3f(1, 0, 0)), w)).normalized();
    Vector3f v = Vector3f::cross(w, u);

    // next, randomly shoot it
    unsigned short Xi[3] = {static_cast<unsigned short>(rand() % 1000), static_cast<unsigned short>(rand() % 100), 0};
    float r1 = 2 * M_PI * erand48(Xi); // angle around
    Xi[0] = 0;
    float r2 = erand48(Xi);
    float r2s = sqrt(r2); // distance from centre
    Vector3f d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalized();
    return d;
}


inline Vector3f random_sphere() {

    // generate a random direction ray in the unit ball
    unsigned short Xi[3] = {static_cast<unsigned short>(rand() % 10), static_cast<unsigned short>(rand() % 1000), 0};
    float x, y, z;
    do {
        x = 2 * erand48(Xi) - 1;
        y = 2 * erand48(Xi) - 1;
        z = 2 * erand48(Xi) - 1;
    } while (x * x + y * y + z * z > 1 || x * x + y * y + z * z < 0);
    return Vector3f(x, y, z).normalized();
}


inline Vector3f rotate(const Vector3f &in, const Vector3f &axis, float angle) {

    // rotate vector in around axis , with an angle
    if (fabs(angle) < 0) return Vector3f(in.x(), in.y(), in.z());
    float cost = cos(angle);
    float sint = sin(angle);

    float x, y, z;
    x += in.x() * (axis.x() * axis.x() + (1 - axis.x() * axis.x()) * cost);
    x += in.y() * (axis.x() * axis.y() * (1 - cost) - axis.z() * sint);
    x += in.z() * (axis.x() * axis.z() * (1 - cost) + axis.y() * sint);
    y += in.x() * (axis.y() * axis.x() * (1 - cost) + axis.z() * sint);
    y += in.y() * (axis.y() * axis.y() + (1 - axis.y() * axis.y()) * cost);
    y += in.z() * (axis.y() * axis.z() * (1 - cost) - axis.x() * sint);
    z += in.x() * (axis.z() * axis.x() * (1 - cost) - axis.y() * sint);
    z += in.y() * (axis.z() * axis.y() * (1 - cost) + axis.x() * sint);
    z += in.z() * (axis.z() * axis.z() + (1 - axis.z() * axis.z()) * cost);

    return Vector3f(x, y, z);
}


inline Vector3f get_normal(const Vector3f &in) {

    // get a normal vector of an input one
    return (in.x() == 0 && in.y() == 0) ?
           Vector3f(1, 0, 0) : Vector3f(in.y(), -in.x(), 0).normalized();
}


inline Vector3f random_cos_sphere(const Vector3f &normal) {

    Vector3f N = normal;
    unsigned short Xi[3] = {static_cast<unsigned short>(rand() % 10), static_cast<unsigned short>(rand() % 1000), 0};
    float theta = acos(sqrt(erand48(Xi)));
    float phi = 2 * M_PI * erand48(Xi);
    Vector3f ret = rotate(N, get_normal(N), theta);
    return rotate(ret, N, phi).normalized();
}


#endif //RAY_TRACER_SIMPLE_METHOD_HPP
