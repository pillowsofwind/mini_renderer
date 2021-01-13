#ifndef SCENE_PARSER_H
#define SCENE_PARSER_H

#include <cassert>
#include <vecmath.h>

#include "camera.hpp"
#include "light.hpp"
#include "object3d.hpp"
#include "group.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "triangle.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "transform.hpp"
#include "cube_map.hpp"
#include "bezier.hpp"

#define MAX_PARSER_TOKEN_LENGTH 100

/*
class Camera;
class Light;
class Material;
class Object3D;
class Group;
class Sphere;
class Plane;
class Triangle;
class Transform;
*/

class SceneParser {
public:

    SceneParser(const char *filename);

    ~SceneParser();

    Camera *getCamera() const {
        return camera;
    }

    Vector3f getBackgroundColor(Vector3f dir) const {
        if (cubemap == 0) {
            return background_color;
        }
        return cubemap->operator()(dir);
    }

    Vector3f getAmbientLight() const {
        return ambient_light;
    }

    int getNumLights() const {
        return num_lights;
    }

    Light *getLight(int i) const {
        assert(i >= 0 && i < num_lights);
        return lights[i];
    }

    int getNumMaterials() const {
        return num_materials;
    }

    Material *getMaterial(int i) const {
        assert(i >= 0 && i < num_materials);
        return materials[i];
    }

    Group *getGroup() const {
        return group;
    }

private:

    SceneParser() {
        assert(false);
    }

    void parseFile();

    void parsePerspectiveCamera();

    void parseBackground();

    void parseLights();

    Light *parseDirectionalLight();

    Light *parsePointLight();

    Light *parseSphereLight();

    void parseMaterials();

    Material *parseMaterial();

    Noise *parseNoise();

    Object3D *parseObject(char token[MAX_PARSER_TOKEN_LENGTH]);

    Group *parseGroup();

    Sphere *parseSphere();

    Plane *parsePlane();

    Triangle *parseTriangle();

    BezierObject *parseBezier();

    Mesh *parseTriangleMesh();

    Transform *parseTransform();

    CubeMap *parseCubeMap();

    int getToken(char token[MAX_PARSER_TOKEN_LENGTH]);

    Vector3f readVector3f();

    Vector2f readVec2f();

    float readfloat();

    int readInt();

    FILE *file;
    Camera *camera;
    Vector3f background_color;
    Vector3f ambient_light;
    int num_lights;
    Light **lights;
    int num_materials;
    Material **materials;
    Material *current_material;
    Group *group;
    CubeMap *cubemap;
};

#endif // SCENE_PARSER_H
