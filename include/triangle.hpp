#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;

class Triangle : public Object3D {

public:
    Vector3f vertices[3];
    Vector3f normals[3];
    bool texture;
    Vector2f texture_coordinates[3];

    Triangle() = delete;

    // a b c are three vertex positions of the triangle
    Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, Material *m) : texture(false), Object3D(m) {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
    }

    Intersection intersect(const Ray &ray, Hit &hit, float tmin) override {

        Vector3f origin = ray.getOrigin();
        Vector3f direction = ray.getDirection();

        float ACramer = Matrix3f(vertices[0].x() - vertices[1].x(), vertices[0].x() - vertices[2].x(), direction.x(),
                                 vertices[0].y() - vertices[1].y(), vertices[0].y() - vertices[2].y(), direction.y(),
                                 vertices[0].z() - vertices[1].z(), vertices[0].z() - vertices[2].z(),
                                 direction.z()).determinant();
        float betaCramer = Matrix3f(vertices[0].x() - origin.x(), vertices[0].x() - vertices[2].x(), direction.x(),
                                    vertices[0].y() - origin.y(), vertices[0].y() - vertices[2].y(), direction.y(),
                                    vertices[0].z() - origin.z(), vertices[0].z() - vertices[2].z(),
                                    direction.z()).determinant();
        float gammaCramer = Matrix3f(vertices[0].x() - vertices[1].x(), vertices[0].x() - origin.x(), direction.x(),
                                     vertices[0].y() - vertices[1].y(), vertices[0].y() - origin.y(), direction.y(),
                                     vertices[0].z() - vertices[1].z(), vertices[0].z() - origin.z(),
                                     direction.z()).determinant();
        float tCramer = Matrix3f(vertices[0].x() - vertices[1].x(), vertices[0].x() - vertices[2].x(),
                                 vertices[0].x() - origin.x(),
                                 vertices[0].y() - vertices[1].y(), vertices[0].y() - vertices[2].y(),
                                 vertices[0].y() - origin.y(),
                                 vertices[0].z() - vertices[1].z(), vertices[0].z() - vertices[2].z(),
                                 vertices[0].z() - origin.z()).determinant();

        float beta = betaCramer / ACramer;
        float gamma = gammaCramer / ACramer;
        float t = tCramer / ACramer;

        if (t >= tmin && t < hit.getT() && beta + gamma <= 1.0 && beta >= 0.0 && gamma >= 0.0) {
            float alpha = 1 - beta - gamma;
            Vector3f normal = alpha * normals[0] + beta * normals[1] + gamma * normals[2];

            if (texture) {
                hit.texture_coordinate =
                        alpha * texture_coordinates[0] + beta * texture_coordinates[1] + gamma * texture_coordinates[2];
                std::cout << hit.texture_coordinate[0] << hit.texture_coordinate[1] << std::endl;
            }

            Vector3f P=ray.pointAtParameter(t);
            hit.set(t, material, normal.normalized(),P, id);

            return OUTSIDE;
        }
        return MISS;
    }

};

#endif //TRIANGLE_H
