//
// Created by XRW's notebook on 2020/6/13.
//

#ifndef RAY_TRACER_CUBE_MAP_HPP
#define RAY_TRACER_CUBE_MAP_HPP

#include "texture.hpp"
#include "vecmath.h"

#include <string>

class CubeMap {
public:
///@brief assumes a directory containing
///left right up down front back.bmp
    CubeMap(const char *dir);

    enum FACE {
        LEFT, RIGHT, UP, DOWN, FRONT, BACK
    };

    Vector3f operator()(const Vector3f &);

    Texture t[6];

};

inline CubeMap::CubeMap(const char *directory) {
    std::string suffix[6] = {"left", "right", "up", "down", "front", "back"};
    std::string dirname(directory);
    for (int ii = 0; ii < 6; ii++) {
        std::string filename = dirname + "/";
        filename = filename + suffix[ii];
        filename = filename + ".bmp";
        t[ii].load(filename.c_str());
    }
}

inline Vector3f CubeMap::operator()(const Vector3f &direction) {
    Vector3f dir = direction.normalized();
    Vector3f outputColor(0.0f, 0.0f, 0.0f);
    if ((fabsf(dir[0]) >= fabsf(dir[1]))
        && (fabsf(dir[0]) >= fabsf(dir[2]))) {
        if (dir[0] > 0.0f) {
            outputColor = t[RIGHT]((dir[2] / dir[0] + 1.0f) * 0.5f,
                                   (dir[1] / dir[0] + 1.0f) * 0.5f);
        } else if (dir[0] < 0.0f) {
            outputColor = t[LEFT]((dir[2] / dir[0] + 1.0f) * 0.5f,
                                  1.0 - (dir[1] / dir[0] + 1.0f) * 0.5f);
        }
    } else if ((fabsf(dir[1]) >= fabsf(dir[0])) && (fabsf(dir[1]) >= fabsf(dir[2]))) {
        if (dir[1] > 0.0f) {
            outputColor = t[UP]((dir[0] / dir[1] + 1.0f) * 0.5f,
                                (dir[2] / dir[1] + 1.0f) * 0.5f);
        } else if (dir[1] < 0.0f) {
            outputColor = t[DOWN](1.0f - (dir[0] / dir[1] + 1.0f) * 0.5f,
                                  1.0f - (dir[2] / dir[1] + 1.0f) * 0.5f);
        }
    } else if ((fabsf(dir[2]) >= fabsf(dir[0]))
               && (fabsf(dir[2]) >= fabsf(dir[1]))) {
        if (dir[2] > 0.0f) {
            outputColor = t[FRONT](1.0f - (dir[0] / dir[2] + 1.0f) * 0.5f,
                                   (dir[1] / dir[2] + 1.0f) * 0.5f);
        } else if (dir[2] < 0.0f) {
            outputColor = t[BACK]((dir[0] / dir[2] + 1.0f) * 0.5f,
                                  1.0f - (dir[1] / dir[2] + 1.0f) * 0.5f);
        }
    }

    return outputColor;
}

#endif //RAY_TRACER_CUBE_MAP_HPP
