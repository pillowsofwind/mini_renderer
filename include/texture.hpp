//
// Created by XRW's notebook on 2020/6/13.
//

#ifndef RAY_TRACER_TEXTURE_HPP
#define RAY_TRACER_TEXTURE_HPP

#include "bitmap_image.hpp"
#include "Vector3f.h"

class Texture {

public:
    bitmap_image *bimg;
    int width, height;

    Texture();

    bool valid();

    void load(const char *filename);

    void operator()(int x, int y, unsigned char *color);

    Vector3f operator()(float x, float y);

    ~Texture();

};

inline void Texture::load(const char *filename) {
    bimg = new bitmap_image(filename);
    height = bimg->height();
    width = bimg->width();
}

inline void Texture::operator()(int x, int y, unsigned char *color) {
    x = clamp(x, 0, width - 1);
    y = clamp(y, 0, height - 1);
    bimg->get_pixel(x, y, color[0], color[1], color[2]);
}

inline bool Texture::valid() {
    return bimg != 0;
}

inline Vector3f Texture::operator()(float x, float y) {
    Vector3f color;
    int ix, iy;
    x = x * width;
    y = (1 - y) * height;
    ix = (int) x;
    iy = (int) y;
    unsigned char pixels[4][3];
    float alpha = x - ix;
    float beta = y - iy;
    operator()(ix, iy, pixels[0]);
    operator()(ix + 1, iy, pixels[1]);
    operator()(ix, iy + 1, pixels[2]);
    operator()(ix + 1, iy + 1, pixels[3]);
    for (int ii = 0; ii < 3; ii++) {
        color[ii] = (1 - alpha) * (1 - beta) * pixels[0][ii]
                    + alpha * (1 - beta) * pixels[1][ii]
                    + (1 - alpha) * beta * pixels[2][ii]
                    + alpha * beta * pixels[3][ii];
    }
    return color / 255;
}

inline Texture::~Texture() {
    if (bimg != 0) {
        delete bimg;
    }
}

inline Texture::Texture() : bimg(0), width(0), height(0) {
}

#endif //RAY_TRACER_TEXTURE_HPP
