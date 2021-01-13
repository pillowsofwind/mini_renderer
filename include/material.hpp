#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <cassert>

#include "vecmath.h"
#include "ray.hpp"
#include "texture.hpp"
#include "hit.hpp"
#include "noise.hpp"
#include "simple_method.hpp"

#define SHININESS 20

enum material_type {
    DIFF, SPEC, REFR
}; // material type

class Material {
public:

    Vector3f diffuseColor;
    Vector3f emissionColor; // needed when do path tracing
    material_type type;
    float refractionIndex;
    Texture t;
    Noise noise;

    Material(const Vector3f &d_color = Vector3f::ZERO,
             const Vector3f &e_color = Vector3f::ZERO, material_type t_ = DIFF, float r = 0)
            : diffuseColor(d_color), emissionColor(e_color), type(t_), refractionIndex(r) {}

    virtual ~Material() = default;

    Vector3f Shade(const Ray &ray, const Hit &hit,
                   const Vector3f &dirToLight, const Vector3f &lightColor);

    void loadTexture(const char *filename) { t.load(filename); }

    float getRefractionIndex() { return refractionIndex; }

    Vector3f getDiffuseColor() { return diffuseColor; }

    Vector3f getType() { return type; }

    Vector3f getEmissionColor() { return emissionColor; }

    void setNoise(const Noise &n) { noise = n; }

};

inline float ReLU(float a) {
    return (a > 0) ? a : 0;
}

inline Vector4f get_weight(material_type type) {

    // Diff,Spec,Refl,Refr,Total=1
    switch (type) {
        case DIFF:
            return Vector4f(1, 0.2, 0.1, 0);
        case SPEC:
            return Vector4f(0.2, 1, 1, 0);
        case REFR:
            return Vector4f(0.2, 0.2, 0.6, 1);
        default:
            return Vector4f(1, 0, 0, 0);
    }
}

inline Vector3f get_spec(material_type type) {

    switch (type) {
        case DIFF:
            return Vector3f(0.02, 0.02, 0.02); // do reflect a little
        case SPEC:
            return Vector3f(0.99, 0.99, 0.99);
        case REFR:
            return Vector3f(0.6, 0.6, 0.6);
        default:
            return Vector3f::ZERO;
    }
}


inline Vector3f Material::Shade(const Ray &ray, const Hit &hit,
                                const Vector3f &dirToLight, const Vector3f &lightColor) {

    // the function here is based on the Phong shading model
    Vector3f kd;

    if (t.valid() && hit.texture) {
        Vector2f texCoord = hit.texture_coordinate;
        Vector3f texColor = t(texCoord[0], texCoord[1]);
        kd = texColor;
    } else {
        kd = this->diffuseColor;
    }
    Vector3f n = hit.getNormal().normalized();

    if (noise.valid()) {
        kd = noise.getColor(ray.getOrigin() + ray.getDirection() * hit.getT());
    }

    //Diffuse Shading
    Vector3f color = (lightColor * kd) * ReLU(Vector3f::dot(dirToLight, n));

    Vector3f ks = get_spec(type);
    Vector3f Rx = 2.f * Vector3f::dot(dirToLight, n) * n - dirToLight;

    //Specular Shading
    color += (lightColor * ks) * pow(ReLU(Vector3f::dot(dirToLight, n)), SHININESS);

    return color;
}

#endif // MATERIAL_H
