#pragma once

#include <vector>
#include <ostream>
#include "PGA/PGA_3D.h"
#include "PGA/image_lib.h"
#include "utils.h"
#include <limits>
#include <optional>

using std::vector;

struct MaterialId {
public:
    explicit MaterialId(uint materialId) : materialId(materialId) {}

    MaterialId() = default;

    uint materialId;
};

struct Material {
    /// ambient color
    Color ambient;

    /// diffuse color
    Color diffuse;

    /// specular color
    Color spectral;

    /// phong cosine power = ns
    uint phong = 5;

    /// transmissive color
    Color transmissive;

    /// index of refraction
    float ior = 1;

    inline friend std::istream &operator>>(std::istream &input, Material &res) {
        input >> res.ambient >> res.diffuse >> res.spectral >> res.phong >> res.transmissive >> res.ior;
        return input;
    }


};

struct Resolution {
    uint width, height;

    inline friend std::istream &operator>>(std::istream &input, Resolution &res) {
        uint w, h;
        input >> w >> h;
        res = {w, h};
        return input;
    }

    inline friend std::ostream &operator<<(std::ostream &os, const Resolution &resolution) {
        os << "width: " << resolution.width << " height: " << resolution.height;
        return os;
    }
};


/**
 * Indexes of vertices
 */
struct Triangle : MaterialId {
    size_t v1{}, v2{}, v3{};

    Triangle(size_t v1, size_t v2, size_t v3) : MaterialId(), v1(v1), v2(v2), v3(v3) {}

    Triangle() = default;


    inline friend std::istream &operator>>(std::istream &input, Triangle &res) {
        size_t a, b, c;
        input >> a >> b >> c;
        res = {a, b, c};
        return input;
    }

};

struct NormalTriangle : MaterialId {
    Triangle triangle;
    size_t n1{}, n2{}, n3{};

    NormalTriangle(const Triangle &triangle, size_t n1, size_t n2, size_t n3) : MaterialId(0), triangle(triangle),
                                                                                n1(n1), n2(n2),
                                                                                n3(n3) {
    }

    NormalTriangle() = default;


    inline friend std::istream &operator>>(std::istream &input, NormalTriangle &res) {
        Triangle tr;
        input >> tr;
        size_t a, b, c;
        input >> a >> b >> c;
        res = {tr, a, b, c};
        return input;
    }

};

struct Plane : MaterialId {
    size_t v1, v2, v3, v4;

    Plane(size_t v1, size_t v2, size_t v3, size_t v4) : MaterialId(), v1(v1), v2(v2), v3(v3), v4(v4) {}
    Plane() = default;

    inline friend std::istream &operator>>(std::istream &input, Plane &res) {
        input >> res.v1 >> res.v2 >> res.v3 >> res.v4;
        return input;
    }
};

struct Box : MaterialId {
    size_t v1, v2, v3, v4;

    Box(size_t v1, size_t v2, size_t v3, size_t v4) : MaterialId(), v1(v1), v2(v2), v3(v3), v4(v4) {}
    Box() = default;

    inline friend std::istream &operator>>(std::istream &input, Box &res) {
        input >> res.v1 >> res.v2 >> res.v3 >> res.v4;
        return input;
    }
};

struct Sphere : MaterialId {

    Sphere(const Point3D &pos, float radius) : MaterialId(), center(pos), radius(radius) {}

    Sphere() = default;

    Point3D center;
    float radius;

    inline friend std::istream &operator>>(std::istream &input, Sphere &res) {
        input >> res.center >> res.radius;
        return input;
    }

};

enum class LightType {
    INVALID,
    POINT,
    DIRECTIONAL,
    SPOT_LIGHT
};

struct Light {

    LightType type;

    explicit Light(LightType type) : type(type) {}

    [[nodiscard]] virtual Color getIntensity(const Point3D &point3D) const {
        Color black;
        return black;
    };

    [[nodiscard]] virtual Dir3D getDirection(const Point3D &point3D) const {
        Dir3D none;
        return none;
    };

    [[nodiscard]] virtual float getDistance(const Point3D &point3D) const {
        return 0.0f;
    }
};

struct DirectionalLight : Light {
    Color color;
    Dir3D direction;

    DirectionalLight(const Color &color, const Dir3D &direction) : Light(LightType::DIRECTIONAL), color(color),
                                                                   direction(direction) {}

    DirectionalLight() : DirectionalLight(Color(), Dir3D()) {
    }

    inline friend std::istream &operator>>(std::istream &input, DirectionalLight &res) {
        input >> res.color >> res.direction;
        res.direction = res.direction.normalized();
        return input;
    }

    [[nodiscard]] Color getIntensity(const Point3D &point3D) const override {
        return color;
    }

    [[nodiscard]] Dir3D getDirection(const Point3D &point3D) const override {
        return direction;
    }

    [[nodiscard]] float getDistance(const Point3D &point3D) const override {
        return std::numeric_limits<float>::infinity();
    }


};


struct PointLight : Light {
    /// color
    Color color;

    /// direction
    Point3D location;

    PointLight(const Color &color, const Point3D &location) : Light(LightType::POINT), color(color),
                                                              location(location) {}

    PointLight() : PointLight(Color(), Point3D()) {}

    inline friend std::istream &operator>>(std::istream &input, PointLight &res) {
        input >> res.color >> res.location;
        return input;
    }

    [[nodiscard]] Color getIntensity(const Point3D &point3D) const override {
        let lightIntensity = color;
        Dir3D dif = (point3D - location);
        const float distanceSquared = dif.magnitudeSqr();
        let loseScalar = distanceSquared;

        // intensity of the light at that point
        let I_L = lightIntensity / loseScalar;
        return I_L;
    }

    [[nodiscard]] Dir3D getDirection(const Point3D &point3D) const override {
        const Dir3D dif = point3D - location;
        return dif.normalized();
    }

    [[nodiscard]] float getDistance(const Point3D &point3D) const override {
        const Dir3D dif = point3D - location;
        return dif.magnitude();
    }

};

struct SpotLight : Light {
    /// color
    Color color;

    /// location
    Point3D location;

    /// direction
    Dir3D direction;

    SpotLight(const Color &color, const Point3D &location, const Dir3D &direction, float angle1,
              float angle2) : Light(LightType::SPOT_LIGHT), color(color), location(location), direction(direction),
                              angle1(angle1),
                              angle2(angle2) {}

    SpotLight() : Light(LightType::SPOT_LIGHT) {}

    /// how the light falls off
    float angle1, angle2;

    inline friend std::istream &operator>>(std::istream &input, SpotLight &res) {

        return input;
    }

};

struct AmbientLight {
    /// color
    uint r, g, b;
};

struct Crop {
    int fromX, fromY, toX, toY;

    Crop() = default;

    Crop(int fromX, int fromY, int toX, int toY) : fromX(fromX), fromY(fromY), toX(toX), toY(toY) {}

    inline friend std::istream &operator>>(std::istream &input, Crop &res) {
        input >> res.fromX >> res.fromY >> res.toX >> res.toY;
        return input;
    }
};


struct InputData {
    Point3D cameraPos = {0, 0, 0};
    Dir3D cameraForward = {0, 0, -1};
    Dir3D cameraUp = {0, 1, 0};
    uint samples = 1;
    Dir3D cameraRight = {};
    float cameraFovHA = 45.0;
    Resolution resolution = {
            .width = 640,
            .height = 480
    };
    std::string outputImage = "raytraced.bmp";
    uint maxVertices = 0;
    uint maxNormals = 0;
    vector<Point3D> vertices{};
    vector<Dir3D> normals{};
    vector<Triangle> triangles{};
    vector<Plane> planes;
    vector<Box> boxes;
    vector<NormalTriangle> normalTriangles{};
    vector <Sphere> spheres{};
    Color background{};
    Crop crop;
    vector <Material> materials{};
    vector <DirectionalLight> directionalLights{};
    vector <PointLight> pointLights;
    vector <SpotLight> spotLights;
    Color ambientLight = {0, 0, 0};
    uint maxDepth = 5;

public:
    [[nodiscard]] vector<const Light *> getLights() const {
        vector<const Light *> tmp;
        tmp.reserve(pointLights.size() + directionalLights.size());

        for (const PointLight &pl : pointLights) {
            tmp.push_back(&pl);
        }

        for (const DirectionalLight &dl : directionalLights) {
            tmp.push_back(&dl);
        }

        return tmp;
    }

};

