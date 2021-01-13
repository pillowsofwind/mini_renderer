//
// Created by XRW's notebook on 2020/6/17.
//

#ifndef RAY_TRACER_BEZIER_HPP
#define RAY_TRACER_BEZIER_HPP

#include "object3d.hpp"
#include "vecmath.h"

#include <algorithm>

#define EPS 1e-4 // adjust this for accuracy

#define DEFAULT_TEXU Vector3f(0,-1,0)
#define DEFAULT_TEXV Vector3f(1,0,0)

struct D { // bounding box features
    double t0, t1;
    double width; // max width of bezier curve
};

class BezierCurve {
public:

    // 2D Bezier curve
    int n; // num of control points
    double *dx, *dy; // coefficients x,y
    double num, max, height, r; // bounding box features

    D data[15];

    BezierCurve(double *px, double *py, int n_, int num_, double r_) : num(num_), n(n_), r(r_) {
        dx = new double[n];
        dy = new double[n];
        n -= 1;

        // preprocess coefficients
        for (int i = 0; i <= n; ++i) {
            dx[i] = px[0];
            dy[i] = py[0];
            for (int j = 0; j <= n - i; ++j) {
                px[j] = px[j + 1] - px[j];
                py[j] = py[j + 1] - py[j];
            }
        }

        double down = 1;
        double next = n;
        double factor = 1;
        for (int i = 0; i <= n; ++i, --next) {
            if (i != 0)
                factor *= i;
            dx[i] = dx[i] * down / factor;
            dy[i] = dy[i] * down / factor;
            down *= next;
        }

        max = 0;
        double interval = 1. / (num - 1);
        double c = 0;
        for (int i = 0; i <= num; c += interval, ++i) {

            // update bounding box features
            data[i].width = 0;
            data[i].t0 = std::max(0.0, c - r);
            data[i].t1 = std::min(1.0, c + r);
            for (double t = data[i].t0; t <= data[i].t1; t += 0.00001) {
                Vector2f pos = get_pos(t);
                if (data[i].width < pos.x())
                    data[i].width = pos.x();
            }
            if (max < data[i].width)
                max = data[i].width;
            data[i].width += EPS;
        }
        max += EPS;
        height = get_pos(1).y();
    }

    Vector2f get_pos(double t) {

        // get position on bezier curve at t
        double ans_x = 0, ans_y = 0, t_pow = 1;
        for (int i = 0; i <= n; ++i) {
            ans_x += dx[i] * t_pow;
            ans_y += dy[i] * t_pow;
            t_pow *= t;
        }
        return Vector2f(ans_x, ans_y);
    }

    Vector2f get_dir(double t) {

        // get derivative on bezier curve at t
        double ans_x = 0, ans_y = 0, t_pow = 1;
        for (int i = 1; i <= n; ++i) {
            ans_x += dx[i] * i * t_pow;
            ans_y += dy[i] * i * t_pow;
            t_pow *= t;
        }
        return Vector2f(ans_x, ans_y);
    }

};

class BezierObject : public Object3D {
public:

    // a revsurface class for 2D bezier curve
    // the curve will rotate around axis x=pos.x and z=pos.z
    BezierCurve curve;
    Vector3f pos; // the bottom center 
    bool texture;

    BezierObject(Vector3f pos_, BezierCurve c_, Material *m, bool t = false) :
            pos(pos_), curve(c_), Object3D(m), texture(t) {}

    double solve_t(double y0);

    virtual Vector2f get_uv(Vector3f point);

    double intersect_sphere(Ray ray, Vector3f center, double radius);

    Intersection intersect(const Ray &r, Hit &h, float tmin) override;

    bool check(double low, double upp, double init, Ray ray, double a, double b, double c, double &final_dis);

    double newton(double t, double a, double b, double c, double low = EPS, double upp = 1 - EPS);

    virtual Vector3f norm(Vector3f p);

};

#endif //RAY_TRACER_BEZIER_HPP
