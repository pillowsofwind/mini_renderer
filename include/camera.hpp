#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <vecmath.h>
#include <cmath>

#define M_PI 3.14159265358979323846
#define DegreesToRadians(x) ((M_PI * x) / 180.0f)

class Camera {
public:

    // these parameters are for Depth of Field
    float focus_dist;
    float len_radius;

    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up);
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
        focus_dist = 0.f;
        len_radius = 0.f;
        srand(time(nullptr));
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;

    virtual ~Camera() = default;

    int getWidth() const { return width; }

    int getHeight() const { return height; }

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

inline const Vector3f random_unit_disk()            //find a random point in unit_disk
{
    // produce 2 random numbers in [-1,1]
    float rand_x, rand_y;
    do {
        rand_x = 2.0f * (rand() % 1000) / 1000.f - 1.0f;
        rand_y = 2.0f * (rand() % 1000) / 1000.f - 1.0f;
    } while (rand_x * rand_x + rand_y * rand_y <= 1);

    return Vector3f(rand_x, rand_y, 0);
}

// You can add new functions or variables whenever needed.
class PerspectiveCamera : public Camera {
public:

    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
                      const Vector3f &up, int imgW, int imgH, float a) : Camera(center, direction, up, imgW, imgH) {
        // angle is in radian.
        angle = a;
    }

    Ray generateRay(const Vector2f &point) override {
        //calculate direction
        // calculate z (with fx = fy in this case )
        float z = tan(0.5 * (M_PI - angle)) * (0.5 * this->getWidth());
        Vector3f dir((point.x() - this->getWidth() / 2), (point.y() - this->getHeight() / 2), z);

        Vector3f center_ = center;
        if (len_radius > 0) {
            Vector3f point_at_focus = dir * focus_dist / z;
            Vector3f point_at_len = random_unit_disk() * len_radius;
            dir = point_at_focus - point_at_len;
            center_ += point_at_len;
        }

        Matrix3f R(horizontal, up, direction);
        dir = R * dir;
        dir.normalize();
        // generate a ray from the center
        return Ray(center_, dir);
    }

protected:
    float angle;
};

#endif //CAMERA_H

