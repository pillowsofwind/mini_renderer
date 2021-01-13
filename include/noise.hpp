//
// Created by XRW's notebook on 2020/6/13.
//

#ifndef RAY_TRACER_NOISE_HPP
#define RAY_TRACER_NOISE_HPP

#include <cmath>
#include <cstdio>

#include "vecmath.h"
#include "perlin_noise.hpp"

class Noise {
public:

    Noise() : octaves(0), init(false) {}

    Noise(int _octaves,
          const Vector3f &color1 = Vector3f::ZERO,
          const Vector3f &color2 = Vector3f(1, 1, 1),
          float freq = 1, float amp = 1);

    Vector3f getColor(const Vector3f &pos);

    bool valid() { return init; }

    Noise(const Noise &n);

    Vector3f color[2];
    int octaves;
    float frequency;
    float amplitude;
    bool init;
};

inline Vector3f Noise::getColor(const Vector3f &pos) {

    float N = PerlinNoise::octaveNoise(pos, octaves);
    float M = sin(frequency * pos.x() + amplitude * N);

    // sin is between -1 and 1, normalize it for interpolation.
    M = (M + 1) / 2.0f;

    return M * color[0] + (1 - M) * color[1];
}

inline Noise::Noise(int _octaves, const Vector3f &color1,
                    const Vector3f &color2, float freq, float amp) :
        octaves(_octaves), frequency(freq), amplitude(amp) {
    color[0] = color1;
    color[1] = color2;
    init = true;
}

inline Noise::Noise(const Noise &n) :
        octaves(n.octaves), frequency(n.frequency),
        amplitude(n.amplitude), init(n.init) {
    color[0] = n.color[0];
    color[1] = n.color[1];
}

#endif //RAY_TRACER_NOISE_HPP
