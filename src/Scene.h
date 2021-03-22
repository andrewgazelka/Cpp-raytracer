#pragma once


#include "InputData.h"
#include "utils.h"
#include <optional>
#include <utility>
#include "primitives/Triangle.h"


using std::vector;

struct HitInformation {
    Point3D location;
    Dir3D normal;
    float t;
    Material material;
};

struct Ray {
    Point3D origin;
    Dir3D direction;

//    operator Line3D() { return Line3D(origin.x, origin.y, origin.z, ); }
};

class Scene {

public:
    explicit Scene(InputData inputData) : inputData(std::move(inputData)) {
        for (const auto &triangle : inputData.triangles) {

            let material = inputData.materials[triangle.materialId];

            let p1 = inputData.vertices[triangle.v1];
            let p2 = inputData.vertices[triangle.v2];
            let p3 = inputData.vertices[triangle.v3];

            const Primitive::Triangle tr = {p1, p2, p3, material};
            triangles.push_back(tr);
        }

        for (const auto &normTriangle : inputData.normalTriangles) {


            let triangle = normTriangle.triangle;

            let material = inputData.materials[triangle.materialId];

            let p1 = inputData.vertices[triangle.v1];
            let p2 = inputData.vertices[triangle.v2];
            let p3 = inputData.vertices[triangle.v3];

            let n1 = inputData.normals[normTriangle.n1];
            let n2 = inputData.normals[normTriangle.n2];
            let n3 = inputData.normals[normTriangle.n3];

            const Primitive::Triangle tr = {p1, p2, p3, n1, n2, n3, material};
            triangles.push_back(tr);
        }
    }

    [[nodiscard]] Ray Reflect(const Ray &ray, const Point3D newOrigin, const Dir3D &normal) const {
        Dir3D reflectedDir = Reflect(ray.direction, normal);
        return {newOrigin, reflectedDir};
    }

    [[nodiscard]] Ray
    Refract(const Ray &ray, float iorIn, float iorOut, Point3D newOrigin, const Dir3D &normal) const;

    [[nodiscard]] Dir3D Reflect(const Dir3D &direction, const Dir3D &normal) const;

    Color DiffuseContribution(const Light *light, const HitInformation &hit);

    Color SpecularContribution(const Light *light, const Ray &viewRay, const HitInformation &hit);


    bool FindIntersection(Ray ray, HitInformation *hitInformation);

    Color EvaluateRayTree(Ray ray, float iorIn = 1.0, int depth = 0);


    [[nodiscard]] std::optional<float>
    raySphereIntersect(Ray ray, const Sphere &sphere, float epsilon = 0.01) const;


private:
    Color ApplyLightingModel(Ray ray, HitInformation hit, float iorIn, float epsilon = 0.01, int depth = 0);
    const InputData inputData;
    std::vector<Primitive::Triangle> triangles;
};

 
