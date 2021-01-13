#include <cstdio>
#include <cstring>
#include <cstdlib>

#define _USE_MATH_DEFINES

#include <cmath>

#include "scene_parser.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "material.hpp"
#include "object3d.hpp"
#include "group.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "triangle.hpp"
#include "transform.hpp"

#define DegreesToRadians(x) ((M_PI * x) / 180.0f)

// define a 2D bezier curve here, not in the txt scene file!
const double bezier_curve_x = 120;
const double bezier_curve_y = 90;
double control_x[] = {20. / bezier_curve_x, 23. / bezier_curve_x, 30. / bezier_curve_x, 30. / bezier_curve_x,
                      30. / bezier_curve_x, 25. / bezier_curve_x, 20. / bezier_curve_x, 15. / bezier_curve_x,
                      30.f / bezier_curve_x};
double control_y[] = {0. / bezier_curve_y, 0. / bezier_curve_y, 10. / bezier_curve_y, 20. / bezier_curve_y,
                      30. / bezier_curve_y, 40. / bezier_curve_y, 60. / bezier_curve_y, 60. / bezier_curve_y,
                      80. / bezier_curve_y};
BezierCurve bezier(control_x, control_y, 9, 9, .365);

SceneParser::SceneParser(const char *filename) {

    // initialize some reasonable default values
    group = NULL;
    camera = NULL;
    background_color = Vector3f(0.5, 0.5, 0.5);
    ambient_light = Vector3f(0, 0, 0);
    num_lights = 0;
    lights = NULL;
    num_materials = 0;
    materials = NULL;
    current_material = NULL;
    cubemap = 0;
    // parse the file
    assert(filename != NULL);
    const char *ext = &filename[strlen(filename) - 4];

    if (strcmp(ext, ".txt") != 0) {
        printf("wrong file name extension\n");
        exit(0);
    }
    file = fopen(filename, "r");

    if (file == NULL) {
        printf("cannot open scene file\n");
        exit(0);
    }
    parseFile();
    fclose(file);
    file = NULL;

    // if no lights are specified, set ambient light to white
    // (do solid color ray casting)
    if (num_lights == 0) {
        printf("WARNING:    No lights specified\n");
        ambient_light = Vector3f(1, 1, 1);
    }
}

SceneParser::~SceneParser() {
    if (group != NULL)
        delete group;
    if (camera != NULL)
        delete camera;
    int i;
    for (i = 0; i < num_materials; i++) {
        delete materials[i];
    }
    delete[] materials;
    for (i = 0; i < num_lights; i++) {
        delete lights[i];
    }
    delete[] lights;
}

// ====================================================================
// ====================================================================

void SceneParser::parseFile() {
    //
    // at the top level, the scene can have a camera,
    // background color and a group of objects
    // (we add lights and other things in future assignments)
    //
    char token[MAX_PARSER_TOKEN_LENGTH];
    while (getToken(token)) {
        if (!strcmp(token, "PerspectiveCamera")) {
            parsePerspectiveCamera();
        } else if (!strcmp(token, "Background")) {
            parseBackground();
        } else if (!strcmp(token, "Lights")) {
            parseLights();
        } else if (!strcmp(token, "Materials")) {
            parseMaterials();
        } else if (!strcmp(token, "Group")) {
            group = parseGroup();
        } else {
            printf("Unknown token in parseFile: '%s'\n", token);
            exit(0);
        }
    }
}

// ====================================================================
// ====================================================================

void SceneParser::parsePerspectiveCamera() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    // read in the camera parameters
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token);
    assert (!strcmp(token, "center"));
    Vector3f center = readVector3f();
    getToken(token);
    assert (!strcmp(token, "direction"));
    Vector3f direction = readVector3f();
    getToken(token);
    assert (!strcmp(token, "up"));
    Vector3f up = readVector3f();
    getToken(token);
    assert (!strcmp(token, "angle"));
    float angle_degrees = readfloat();
    float angle_radians = DegreesToRadians(angle_degrees);
    getToken(token);
    assert (!strcmp(token, "width"));
    int width = readInt();
    getToken(token);
    assert (!strcmp(token, "height"));
    int height = readInt();
    getToken(token);
    assert (!strcmp(token, "}"));
    camera = new PerspectiveCamera(center, direction, up, width, height, angle_radians);
}

void SceneParser::parseBackground() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    // read in the background color
    getToken(token);
    assert (!strcmp(token, "{"));
    while (1) {
        getToken(token);
        if (!strcmp(token, "}")) {
            break;
        } else if (!strcmp(token, "color")) {
            background_color = readVector3f();
        } else if (!strcmp(token, "ambientLight")) {
            ambient_light = readVector3f();
        } else if (strcmp(token, "cubeMap") == 0) {
            cubemap = parseCubeMap();
        } else {
            printf("Unknown token in parseBackground: '%s'\n", token);
            assert(0);
        }
    }
}

CubeMap *SceneParser::parseCubeMap() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    return new CubeMap(token);
}
// ====================================================================
// ====================================================================

void SceneParser::parseLights() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));
    // read in the number of objects
    getToken(token);
    assert (!strcmp(token, "numLights"));
    num_lights = readInt();
    lights = new Light *[num_lights];
    // read in the objects
    int count = 0;
    while (num_lights > count) {
        getToken(token);
        if (!strcmp(token, "DirectionalLight")) {
            lights[count] = parseDirectionalLight();
        } else if (strcmp(token, "PointLight") == 0) {
            lights[count] = parsePointLight();
        } else if (strcmp(token, "SphereLight") == 0) {
            lights[count] = parseSphereLight();
        } else {
            printf("Unknown token in parseLight: '%s'\n", token);
            exit(0);
        }
        count++;
    }
    getToken(token);
    assert (!strcmp(token, "}"));
}


Light *SceneParser::parseDirectionalLight() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token);
    assert (!strcmp(token, "direction"));
    Vector3f direction = readVector3f();
    getToken(token);
    assert (!strcmp(token, "color"));
    Vector3f color = readVector3f();
    getToken(token);
    assert (!strcmp(token, "}"));
    return new DirectionalLight(direction, color);
}

Light *SceneParser::parsePointLight() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    Vector3f position, color;
    float falloff = 0;
    getToken(token);
    assert (!strcmp(token, "{"));
    while (1) {
        getToken(token);
        if (strcmp(token, "position") == 0) {
            position = readVector3f();
        } else if (strcmp(token, "color") == 0) {
            color = readVector3f();
        } else if (strcmp(token, "falloff") == 0) {
            falloff = readfloat();
        } else {
            assert (!strcmp(token, "}"));
            break;
        }
    }
    return new PointLight(position, color, falloff);
}

Light *SceneParser::parseSphereLight() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    Vector3f position, color;
    float radius = 1; // unit sphere light
    getToken(token);
    assert (!strcmp(token, "{"));
    while (1) {
        getToken(token);
        if (strcmp(token, "position") == 0) {
            position = readVector3f();
        } else if (strcmp(token, "color") == 0) {
            color = readVector3f();
        } else if (strcmp(token, "radius") == 0) {
            radius = readfloat();
        } else {
            assert (!strcmp(token, "}"));
            break;
        }
    }
    return new SphereLight(position, color, radius);
}

// ====================================================================
// ====================================================================

void SceneParser::parseMaterials() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));
    // read in the number of objects
    getToken(token);
    assert (!strcmp(token, "numMaterials"));
    num_materials = readInt();
    materials = new Material *[num_materials];
    // read in the objects
    int count = 0;
    while (num_materials > count) {
        getToken(token);
        if (!strcmp(token, "Material") ||
            !strcmp(token, "PhongMaterial")) {
            materials[count] = parseMaterial();
        } else {
            printf("Unknown token in parseMaterial: '%s'\n", token);
            exit(0);
        }
        count++;
    }
    getToken(token);
    assert (!strcmp(token, "}"));
}


Material *SceneParser::parseMaterial() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    char filename[MAX_PARSER_TOKEN_LENGTH];
    filename[0] = 0;
    Vector3f diffuseColor(1, 1, 1), emissionColor(0, 0, 0);
    material_type type = DIFF;
    float refractionIndex = 0;
    getToken(token);
    assert (!strcmp(token, "{"));
    Noise *noise = 0;
    while (1) {
        getToken(token);
        if (strcmp(token, "diffuseColor") == 0) {
            diffuseColor = readVector3f();
        } else if (strcmp(token, "emissionColor") == 0) {
            emissionColor = readVector3f();
        } else if (strcmp(token, "type") == 0) {
            getToken(token);
            if (strcmp(token, "DIFF") == 0) {
                type = DIFF;
            } else if (strcmp(token, "SPEC") == 0) {
                type = SPEC;
            } else if (strcmp(token, "REFR") == 0) {
                type = REFR;
            } else {
                printf("Unknown token in type: '%s'\n", token);
                exit(0);
            }
        } else if (strcmp(token, "refractionIndex") == 0) {
            refractionIndex = readfloat();
        } else if (strcmp(token, "texture") == 0) {
            getToken(filename);
        }
            ///unimplemented
        else if (strcmp(token, "bump") == 0) {
            getToken(token);
        } else if (strcmp(token, "Noise") == 0) {
            noise = parseNoise();
        } else {
            assert (!strcmp(token, "}"));
            break;
        }
    }
    Material *answer = new Material(diffuseColor, emissionColor, type, refractionIndex);
    if (filename[0] != 0) {
        answer->loadTexture(filename);
        printf("Successfully load texture for material!\n");
    }
    if (noise != 0) {
        answer->setNoise(*noise);
        printf("Successfully load noise for material!\n");
        delete noise;
    }
    return answer;
}

Noise *SceneParser::parseNoise() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    Vector3f color[2];
    int colorIdx = 0;
    int octaves = 0;
    float frequency = 1;
    float amplitude = 1;
    getToken(token);
    assert (!strcmp(token, "{"));
    Noise *noise = 0;
    while (1) {
        getToken(token);
        if (strcmp(token, "color") == 0) {
            if (colorIdx > 1) {
                printf("Error parsing noise\n");
            } else {
                color[colorIdx] = readVector3f();
                colorIdx++;
            }
        } else if (strcmp(token, "octaves") == 0) {
            octaves = readInt();
        } else if (strcmp(token, "frequency") == 0) {
            frequency = readfloat();
        } else if (strcmp(token, "amplitude") == 0) {
            amplitude = readfloat();
        } else {
            assert (!strcmp(token, "}"));
            break;
        }
    }
    return new Noise(octaves, color[0], color[1], frequency, amplitude);
}
// ====================================================================
// ====================================================================

Object3D *SceneParser::parseObject(char token[MAX_PARSER_TOKEN_LENGTH]) {
    Object3D *answer = NULL;
    if (!strcmp(token, "Group")) {
        answer = (Object3D *) parseGroup();
    } else if (!strcmp(token, "Sphere")) {
        answer = (Object3D *) parseSphere();
    } else if (!strcmp(token, "Plane")) {
        answer = (Object3D *) parsePlane();
    } else if (!strcmp(token, "Triangle")) {
        answer = (Object3D *) parseTriangle();
    } else if (!strcmp(token, "TriangleMesh")) {
        answer = (Object3D *) parseTriangleMesh();
    } else if (!strcmp(token, "Transform")) {
        answer = (Object3D *) parseTransform();
    } else if (!strcmp(token, "Bezier")) {
        answer = (Object3D *) parseBezier();
    } else {
        printf("Unknown token in parseObject: '%s'\n", token);
        exit(0);
    }
    return answer;
}

// ====================================================================
// ====================================================================

Group *SceneParser::parseGroup() {
    //
    // each group starts with an integer that specifies
    // the number of objects in the group
    //
    // the material index sets the material of all objects which follow,
    // until the next material index (scoping for the materials is very
    // simple, and essentially ignores any tree hierarchy)
    //
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));

    // read in the number of objects
    getToken(token);
    assert (!strcmp(token, "numObjects"));
    int num_objects = readInt();

    Group *answer = new Group(num_objects);

    // read in the objects
    int count = 0;
    while (num_objects > count) {
        getToken(token);
        if (!strcmp(token, "MaterialIndex")) {
            // change the current material
            int index = readInt();
            assert (index >= 0 && index <= getNumMaterials());
            current_material = getMaterial(index);
        } else {
            Object3D *object = parseObject(token);
            assert (object != NULL);
            answer->addObject(count, object);

            count++;
        }
    }
    getToken(token);
    assert (!strcmp(token, "}"));

    // return the group
    return answer;
}

// ====================================================================
// ====================================================================

BezierObject *SceneParser::parseBezier() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    bool texture = false;
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token);
    assert (!strcmp(token, "center"));
    Vector3f center = readVector3f();
    getToken(token);
    if (strcmp(token, "texture") == 0) {
        texture = readInt();
        getToken(token);
    }
    assert (!strcmp(token, "}"));
    assert (current_material != NULL);
    return new BezierObject(center, bezier, current_material, texture);
}


Sphere *SceneParser::parseSphere() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    bool texture = false;
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token);
    assert (!strcmp(token, "center"));
    Vector3f center = readVector3f();
    getToken(token);
    assert (!strcmp(token, "radius"));
    float radius = readfloat();
    getToken(token);
    if (strcmp(token, "texture") == 0) {
        texture = readInt();
        getToken(token);
    }
    assert (!strcmp(token, "}"));
    assert (current_material != NULL);
    return new Sphere(center, radius, current_material, texture);
}


Plane *SceneParser::parsePlane() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    bool texture = false;
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token);
    assert (!strcmp(token, "normal"));
    Vector3f normal = readVector3f();
    getToken(token);
    assert (!strcmp(token, "offset"));
    float offset = readfloat();
    getToken(token);
    if (strcmp(token, "texture") == 0) {
        texture = readInt();
        getToken(token);
    }
    assert (!strcmp(token, "}"));
    assert (current_material != NULL);
    return new Plane(normal, offset, current_material, texture);
}


Triangle *SceneParser::parseTriangle() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token);
    assert (!strcmp(token, "vertex0"));
    Vector3f v0 = readVector3f();
    getToken(token);
    assert (!strcmp(token, "vertex1"));
    Vector3f v1 = readVector3f();
    getToken(token);
    assert (!strcmp(token, "vertex2"));
    Vector3f v2 = readVector3f();
    getToken(token);
    assert (!strcmp(token, "}"));
    assert (current_material != NULL);
    return new Triangle(v0, v1, v2, current_material);
}


Mesh *SceneParser::parseTriangleMesh() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    char filename[MAX_PARSER_TOKEN_LENGTH];
    // get the filename
    getToken(token);
    assert (!strcmp(token, "{"));
    getToken(token);
    assert (!strcmp(token, "obj_file"));
    getToken(filename);
    getToken(token);
    assert (!strcmp(token, "}"));
    const char *ext = &filename[strlen(filename) - 4];
    assert(!strcmp(ext, ".obj"));
    Mesh *answer = new Mesh(filename, current_material);

    return answer;
}


Transform *SceneParser::parseTransform() {
    char token[MAX_PARSER_TOKEN_LENGTH];
    Matrix4f matrix = Matrix4f::identity();
    Object3D *object = NULL;
    getToken(token);
    assert (!strcmp(token, "{"));
    // read in transformations:
    // apply to the LEFT side of the current matrix (so the first
    // transform in the list is the last applied to the object)
    getToken(token);

    while (1) {
        if (!strcmp(token, "Scale")) {
            Vector3f s = readVector3f();
            matrix = matrix * Matrix4f::scaling(s[0], s[1], s[2]);
        } else if (!strcmp(token, "UniformScale")) {
            float s = readfloat();
            matrix = matrix * Matrix4f::uniformScaling(s);
        } else if (!strcmp(token, "Translate")) {
            matrix = matrix * Matrix4f::translation(readVector3f());
        } else if (!strcmp(token, "XRotate")) {
            matrix = matrix * Matrix4f::rotateX(DegreesToRadians(readfloat()));
        } else if (!strcmp(token, "YRotate")) {
            matrix = matrix * Matrix4f::rotateY(DegreesToRadians(readfloat()));
        } else if (!strcmp(token, "ZRotate")) {
            matrix = matrix * Matrix4f::rotateZ(DegreesToRadians(readfloat()));
        } else if (!strcmp(token, "Rotate")) {
            getToken(token);
            assert (!strcmp(token, "{"));
            Vector3f axis = readVector3f();
            float degrees = readfloat();
            float radians = DegreesToRadians(degrees);
            matrix = matrix * Matrix4f::rotation(axis, radians);
            getToken(token);
            assert (!strcmp(token, "}"));
        } else if (!strcmp(token, "Matrix4f")) {
            Matrix4f matrix2 = Matrix4f::identity();
            getToken(token);
            assert (!strcmp(token, "{"));
            for (int j = 0; j < 4; j++) {
                for (int i = 0; i < 4; i++) {
                    float v = readfloat();
                    matrix2(i, j) = v;
                }
            }
            getToken(token);
            assert (!strcmp(token, "}"));
            matrix = matrix2 * matrix;
        } else {
            // otherwise this must be an object,
            // and there are no more transformations
            object = parseObject(token);
            break;
        }
        getToken(token);
    }

    assert(object != NULL);
    getToken(token);
    assert (!strcmp(token, "}"));
    return new Transform(matrix, object);
}

// ====================================================================
// ====================================================================

int SceneParser::getToken(char token[MAX_PARSER_TOKEN_LENGTH]) {
    // for simplicity, tokens must be separated by whitespace
    assert (file != NULL);
    int success = fscanf(file, "%s ", token);
    if (success == EOF) {
        token[0] = '\0';
        return 0;
    }
    return 1;
}


Vector3f SceneParser::readVector3f() {
    float x, y, z;
    int count = fscanf(file, "%f %f %f", &x, &y, &z);
    if (count != 3) {
        printf("Error trying to read 3 floats to make a Vector3f\n");
        assert (0);
    }
    return Vector3f(x, y, z);
}


Vector2f SceneParser::readVec2f() {
    float u, v;
    int count = fscanf(file, "%f %f", &u, &v);
    if (count != 2) {
        printf("Error trying to read 2 floats to make a Vec2f\n");
        assert (0);
    }
    return Vector2f(u, v);
}


float SceneParser::readfloat() {
    float answer;
    int count = fscanf(file, "%f", &answer);
    if (count != 1) {
        printf("Error trying to read 1 float\n");
        assert (0);
    }
    return answer;
}


int SceneParser::readInt() {
    int answer;
    int count = fscanf(file, "%d", &answer);
    if (count != 1) {
        printf("Error trying to read 1 int\n");
        assert (0);
    }
    return answer;
}
