//
// Created by XRW's notebook on 2020/6/17.
//

#include "bezier.hpp"

double BezierObject::newton(double t, double a, double b, double c, double low, double upp) {

    // solve sqrt(a(y(t)+pos.y-b)^2+c)=x(t)
    // f(t) = x(t) - sqrt(a(y(t)+pos.y-b)^2+c)
    // f'(t) = x'(t) - a(y(t)+pos.y-b)*y'(t) / sqrt(...)
    // if t is not in [0, 1] then assume f(t) is a linear function
    double ft, dft, x, y, dx, dy, sq;
    Vector2f loc, dir;
    for (int i = 10; i--;) {
        if (t < 0) t = low;
        if (t > 1) t = upp;
        loc = curve.get_pos(t), dir = curve.get_dir(t);
        x = loc.x(), dx = dir.x();
        y = loc.y(), dy = dir.y();

        sq = sqrt(a * (y - b) * (y - b) + c);
        ft = x - sq;
        dft = dx - a * (y - b) * dy / sq;
        if (std::abs(ft) < EPS)
            return t;
        t -= ft / dft;
    }
    return -1;
}


Intersection BezierObject::intersect(const Ray &r, Hit &h, float tmin) {

    double final_dis = 1e38;
    // check for |dy| < eps
    if (std::abs(r.getDirection().y()) < 5e-4) {
        double dis_to_axis = (Vector3f(pos.x(), r.getOrigin().y(), pos.z()) - r.getOrigin()).length();
        double hit = r.pointAtParameter(dis_to_axis).y();
        if (hit < pos.y() + EPS || hit > pos.y() + curve.height - EPS)
            return MISS;

        // solve function pos.y+y(t)=ray.o.y() to get x(t)
        double t = solve_t(hit - pos.y());
        if (t < 0 || t > 1)
            return MISS;
        Vector2f loc = curve.get_pos(t);
        double ft = pos.y() + loc.y() - hit;
        if (std::abs(ft) > EPS)
            return MISS;

        // assume sphere (pos.x, pos.y + loc.y, pos.z) - loc.x
        final_dis = intersect_sphere(r, Vector3f(pos.x(), pos.y() + loc.y(), pos.z()), loc.x());
        if (final_dis < 0)
            return MISS;
        Vector3f inter_p = r.pointAtParameter(final_dis);

        if (std::abs((inter_p - Vector3f(pos.x(), inter_p.y(), pos.z())).squaredLength() - loc.x() * loc.x()) >
            1e-1)
            return MISS;

        // one more iteration for more accuracy
        hit = inter_p.y();
        if (hit < pos.y() + EPS || hit > pos.y() + curve.height - EPS)
            return MISS;
        t = solve_t(hit - pos.y());
        loc = curve.get_pos(t);
        ft = pos.y() + loc.y() - hit;
        if (std::abs(ft) > EPS)
            return MISS;
        final_dis = intersect_sphere(r, Vector3f(pos.x(), pos.y() + loc.y(), pos.z()), loc.x());
        if (final_dis < 0)
            return MISS;
        inter_p = r.pointAtParameter(final_dis);
        if (std::abs((inter_p - Vector3f(pos.x(), hit, pos.z())).squaredLength() - loc.x() * loc.x()) > 1e-2)
            return MISS;
        if (final_dis > h.getT())
            return MISS;
        Vector3f n = norm(inter_p);
        h.set(final_dis, material, norm(inter_p), inter_p, id);
        if (texture) {
            float theta = acos(-Vector3f::dot(n, DEFAULT_TEXV));
            float phi = acos(std::min(std::max(Vector3f::dot(n, DEFAULT_TEXU) / sin(theta), -1.f), 1.f));
            float u = theta / M_PI, v = phi / (2 * M_PI);
            v = (Vector3f::dot(n, Vector3f::cross(DEFAULT_TEXU, DEFAULT_TEXV)) < 0) ? (1 - v) : v;
            h.texture = true;
            h.texture_coordinate = Vector2f(u, v);
        }
        return OUTSIDE;
    }

    // case ay^2+by+c
    double a = 0, b = 0, c = 0, t1, t2;
    // (xo-x'+xd/yd*(y-yo))^2 -> (t1+t2*y)^2
    t1 = r.getOrigin().x() - pos.x() - r.getDirection().x() / r.getDirection().y() * r.getOrigin().y();
    t2 = r.getDirection().x() / r.getDirection().y();
    a += t2 * t2;
    b += 2 * t1 * t2;
    c += t1 * t1;
    // (zo-z'+zd/yd*(y-yo))^2 -> (t1+t2*y)^2
    t1 = r.getOrigin().z() - pos.z() - r.getDirection().z() / r.getDirection().y() * r.getOrigin().y();
    t2 = r.getDirection().z() / r.getDirection().y();
    a += t2 * t2;
    b += 2 * t1 * t2;
    c += t1 * t1;
    // ay^2+by+c -> a'(y-b')^2+c'
    c = c - b * b / 4 / a;
    b = -b / 2 / a - pos.y();
    if ((0 <= b && b <= curve.height &&
         c > curve.max * curve.max) ||
        ((b < 0 || b > curve.height) &&
         std::min(b * b, (curve.height - b) * (curve.height - b)) * a + c > curve.max * curve.max)) // no intersect
        return MISS;
    for (int ind = 0; ind <= curve.num; ++ind) {
        double t0 = curve.data[ind].t0, t1 = curve.data[ind].t1;
        {
            check(t0, t1, (t0 + t1 + t0) / 3, r, a, b, c, final_dis);
            check(t0, t1, (t1 + t0 + t1) / 3, r, a, b, c, final_dis);
        }
    }
    if (final_dis < 1e38 / 2) {
        Vector3f n = norm(r.pointAtParameter(final_dis));
        if (texture) {
            float theta = acos(-Vector3f::dot(n, DEFAULT_TEXV));
            float phi = acos(std::min(std::max(Vector3f::dot(n, DEFAULT_TEXU) / sin(theta), -1.f), 1.f));
            float u = theta / M_PI, v = phi / (2 * M_PI);
            v = (Vector3f::dot(n, Vector3f::cross(DEFAULT_TEXU, DEFAULT_TEXV)) < 0) ? (1 - v) : v;
            h.texture = true;
            h.texture_coordinate = Vector2f(u, v);
        }
        h.set(final_dis, material, n, r.pointAtParameter(final_dis), id);
        return OUTSIDE;
    } else {
        return MISS;
    }
}


bool
BezierObject::check(double low, double upp, double init, Ray ray, double a, double b, double c, double &final_dis) {

    double t = newton(init, a, b, c, low, upp);
    if (t <= 0 || t >= 1)
        return false;
    Vector2f loc = curve.get_pos(t);
    double x = loc.x(), y = loc.y();
    double ft = x - sqrt(a * (y - b) * (y - b) + c);
    if (std::abs(ft) > EPS)
        return false;
    // calc t for ray
    double dis = (pos.y() + y - ray.getOrigin().y()) / ray.getDirection().y();
    if (dis < EPS)
        return false;
    Vector3f inter_p = ray.pointAtParameter(dis);
    if (std::abs((Vector3f(pos.x(), pos.y() + y, pos.z()) - inter_p).squaredLength() - x * x) > EPS)
        return false;
    if (dis < final_dis) {
        final_dis = dis;
        return true;
    }
    return false;
}


Vector3f BezierObject::norm(Vector3f p) {

    // get normal of bezier object at p
    Vector2f tmp = get_uv(p);
    Vector2f dir = curve.get_dir(tmp.y());
    Vector3f d_surface = Vector3f(cos(tmp.x()), dir.y() / dir.x(), sin(tmp.x()));
    Vector3f d_circ = Vector3f(-sin(tmp.x()), 0, cos(tmp.x()));
    return (Vector3f::cross(d_circ, d_surface)).normalized();
}


double BezierObject::solve_t(double y0) {

    // solve function y(t)=y0
    double t = .5;
    double ft, dft;
    for (int i = 10; i--;) {
        if (t < 0) t = 0;
        else if (t > 1) t = 1;
        ft = curve.get_pos(t).y() - y0, dft = curve.get_dir(t).y();
        if (std::abs(ft) < EPS)
            return t;
        t -= ft / dft;
    }
    return -1;
}


Vector2f BezierObject::get_uv(Vector3f point) {

    // return u,v for normal or texture
    double t = solve_t(point.y() - pos.y());
    // arctan(y/x)
    double u = atan2(point.z() - pos.z(), point.x() - pos.x());
    // u is between -pi ~ pi
    if (u < 0)
        u += 2 * M_PI;
    return Vector2f(u, t);
}


double BezierObject::intersect_sphere(Ray ray, Vector3f center, double radius) {

    // test intersection with sphere (center,radius)
    Vector3f OC = center - ray.getOrigin();
    Vector3f dir = ray.getDirection();
    dir.normalize();
    double b = Vector3f::dot(dir, OC);
    double det = b * b - Vector3f::dot(OC, OC) + radius * radius;
    if (det < 0)
        return -1;
    else det = sqrt(det);
    double t = b - det > EPS ? b - det : b + det > EPS ? b + det : -1;
    if (t < 0)
        return -1;
    return t;
}
