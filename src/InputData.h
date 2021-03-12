#pragma once

#include <vector>
#include "PGA/PGA_3D.h"

using std::vector;


struct Resolution {
    float width, height;
};


/**
 * Indexes of vertices
 */
struct Triangle {
    size_t v1{}, v2{}, v3{};
};

struct NormalTriangle {
    Triangle triangle;
    size_t n1{}, n2{}, n3{};
};

struct Sphere {
    float x, y, z, r;
};

struct Background {
    uint r, g, b;
};

struct Material {
    /// ambient color
    uint ar{}, ag{}, ab{};

    /// diffuse color
    uint dr = 1, dg = 1, db = 1;

    /// specular color
    uint sr{}, sg{}, sb{};

    /// phong cosine power
    uint ns = 5;

    /// transmissive color
    uint tr{}, tg{}, tb{};

    /// index of refraction
    uint ior = 1;

};

struct DirectionalLight {
    /// color
    uint r, g, b;

    /// direction
    float x, y, z;
};

struct PointLight {
    /// color
    uint r, g, b;

    /// direction
    float x, y, z;
};

struct SpotLight {
    /// color
    uint r, g, b;

    /// location
    float px, py, pz;

    /// direction
    float dx, dy, z;

    /// how the light falls off
    float angle1, angle2;
};

struct AmbientLight {
    /// color
    uint r, g, b;
};

struct InputData {
    Point3D cameraPos = {0, 0, 0};
    Dir3D cameraFwd = {0, 0, 1};
    Dir3D cameraUp = {0, 1, 0};
    float cameraHA = 45.0;
    Resolution resolution = {
            .width = 640,
            .height = 480
    };
    string outputImage = "raytraced.bmp";
    int maxVertices = 0;
    int maxNormals = 0;
    vector<Point3D> vertices{};
    vector<Point3D> normals{};
    vector<Triangle> triangles{};
    vector<NormalTriangle> normalTriangles{};
    vector<Sphere> spheres{};
    Background background{};
    vector<Material> materials{};
    vector<DirectionalLight> directionalLights{};
    vector<PointLight> pointLights;
    vector<SpotLight> spotLights;
    AmbientLight ambientLight = {0, 0, 0};
    uint maxDepth = 5;
};

