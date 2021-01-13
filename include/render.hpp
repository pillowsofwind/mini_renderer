//
// Created by XRW's notebook on 2020/6/13.
//

#ifndef RENDER_HPP
#define RENDER_HPP

#include "group.hpp"
#include "image.hpp"
#include "scene_parser.hpp"
#include "camera.hpp"
#include "scene_parser.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include "material.hpp"
#include "light.hpp"
#include "simple_method.hpp"
#include "kdtree.hpp"

#include <cassert>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include <iostream>
//#include <omp.h>

// one for all max bounces/depth
#define MAX_BOUNCES 8

// PPM parameters
#define MAX_PPM_ITER 8000
#define MAX_PHOTON_NUM 500000
#define INIT_RADIUS 2
#define ALPHA 0.5

// depth of field features, set this according to the scene
#define FOCUS_DIST 8
#define LEN_RADIUS 0.012

// volumetric light switch：1 on
#define VOLUMETRIC 0

// path tracing features
#define SAMPLES 200 // samples per pixels = samples*9 (in path tracing)

#define EPS 1e-4


class Render {
public:

    SceneParser *parser;
    Group *group;
    int max_bounces;
    char *output;
    float len_radius; // radius for camera lens
    float focus_dist; // focus dist for camera lens
    float init_radius; // ppm initial radius
    float alpha;
    KDTree tree;
    bool volumetic;
    Sphere media; // homogeneous media
    float sigma_s; // scatter
    float sigma_a; // absorption
    float sigma_t;
    std::vector<std::vector<Vector3f>> pixels;
    std::vector<HitPoint> hit_points;
    std::vector<HitPoint> bg_hit_points; // background hit point

    explicit Render(SceneParser *p, char *out) : parser(p), output(out), max_bounces(MAX_BOUNCES),
                                                 len_radius(LEN_RADIUS), focus_dist(FOCUS_DIST),
                                                 init_radius(INIT_RADIUS), alpha(ALPHA), volumetic(VOLUMETRIC) {
        group = parser->getGroup();
        // set the parameters according to the scene
        Vector3f center(2, 2, 3);
        Material *material=new Material;
        material->type = DIFF;
        media = Sphere(center, 10, material);
        delete material;
    }

    ~Render() {}

//    static Vector3f get_pixel(const Image &img, int x, int y) {
//        if (x < 0) x = 0;
//        if (x >= img.Width()) x = img.Width() - 1;
//        if (y < 0) y = 0;
//        if (y >= img.Height()) y = img.Height() - 1;
//
//        return img.GetPixel(x, y);
//    }

    void render(int type = 0);

    // basic ray tracing
    Vector3f trace(const Ray &ray, HitPoint hit, float tmin, int bounces);

    // monte carlo path tracing
    Vector3f path_trace(const Ray &ray, float tmin, int bounces, unsigned short *Xi, int E = 1);

    // monte carlo path tracing with volumetric effect
    Vector3f path_trace_plus(const Ray &ray, float tmin, int bounces, unsigned short *Xi);

    // progressive photon mapping
    void trace_photon(Photon &photon, int depth);

    // update kdtree
    void update_kdtree();

    // evaluate radience
    void evaluate(int iter);

    // save bmp image
    void save_image();
};


#endif //FINAL_RENDER_HPP
