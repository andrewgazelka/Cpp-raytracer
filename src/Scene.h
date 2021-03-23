#pragma once


#include "InputData.h"
#include "utils.h"
#include <optional>
#include <utility>
#include "primitives/Triangle.h"
#include "primitives/PrimitiveProcessor.h"


using std::vector;

enum Shape {
    SPHERE,
    TRIANGLE
};

/**
 * @param location The location of the hit
 * @param t the t-value given a normalized ray it hit from
 * @param Dir3D a normal pointing OUT of the object
 */
struct HitInformation {
    Point3D location;
    Dir3D normal;
    float originX = -1.0;
    float originY = -1.0;
    float t = -1.0;
    Material material;
    Shape shape = SPHERE;
};

struct Ray {
    Point3D origin;
    Dir3D direction;

//    operator Line3D() { return Line3D(origin.x, origin.y, origin.z, ); }
};

class Scene {

public:
    explicit Scene(const InputData &inputData) : inputData(inputData) {
        triangles = PrimitiveProcessor::process(inputData);
    }

    [[nodiscard]] Ray Reflect(const Ray &ray, const Point3D newOrigin, const Dir3D &normal) const {
        Dir3D reflectedDir = Reflect(ray.direction, normal);
        return {newOrigin, reflectedDir};
    }

    [[nodiscard]] Ray
    Refract(const Ray &ray, const Material &material, const Point3D &newOrigin, const Dir3D &normal) const;

    [[nodiscard]] Dir3D ReflectPointingAway(const Dir3D &direction, const Dir3D &normal) const;

    Color DiffuseContribution(const Light *light, const HitInformation &hit);

    Color SpecularContribution(const Light *light, const Ray &viewRay, const HitInformation &hit) const;


    bool FindIntersection(Ray ray, float originX, float originY, HitInformation *hitInformation, float epsilon);


    [[nodiscard]] std::optional<float>
    raySphereIntersect(const Ray &ray, const Sphere &sphere, float epsilon = 0.01) const;

    Color EvaluateRayTree(const Ray &ray, float originX, float originY) {
        return EvaluateRayTree(ray, 0, originX, originY);
    }

private:

    vector<Primitive::Triangle> triangles;

    Color EvaluateRayTree(const Ray &ray, int depth, float originX, float originY);

    Color ApplyLightingModel(const Ray &ray, const HitInformation &hit, float epsilon = 0.01, int depth = 0);

    const InputData inputData;

    Dir3D Reflect(const Dir3D &direction, const Dir3D &normal) const;

};

 
