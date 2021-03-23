#pragma once


#include <InputData.h>
#include "Triangle.h"

class PrimitiveProcessor {
public:

    void process(){

    }

    static vector<Primitive::Triangle> process(const InputData &inputData) {

        PrimitiveProcessor pp;

        for (const auto &triangle : inputData.triangles) {

            let material = inputData.materials[triangle.materialId];

            let p1 = inputData.vertices[triangle.v1];
            let p2 = inputData.vertices[triangle.v2];
            let p3 = inputData.vertices[triangle.v3];

            pp.addTriangle(p1, p2, p3, material);
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

            pp.addNormalTriangle(p1, p2, p3, n1, n2, n3, material);
        }

        for (const auto &plane : inputData.planes) {
            let material = inputData.materials[plane.materialId];

            let p1 = inputData.vertices[plane.v1];
            let p2 = inputData.vertices[plane.v2];
            let p3 = inputData.vertices[plane.v3];
            let p4 = inputData.vertices[plane.v4];

            pp.addPlane(p1, p2, p3, p4, material);
        }

        for (const auto &box: inputData.boxes) {

            let material = inputData.materials[box.materialId];

            let p1 = inputData.vertices[box.v1];
            let p2 = inputData.vertices[box.v2];
            let p3 = inputData.vertices[box.v3];
            let p4 = inputData.vertices[box.v4];

            pp.addBox(p1, p2, p3, p4, material);
        }

        return pp.triangles;

    }

    vector<Primitive::Triangle> triangles;

private:
    explicit PrimitiveProcessor() = default;

    void addTriangle(Point3D p1, Point3D p2, Point3D p3, Material material) {
        const Primitive::Triangle tr = {p1, p2, p3, material};
        triangles.push_back(tr);
    }

    void addBox(Point3D v1, Point3D v2, Point3D v3, Point3D v4, Material material) {
        Dir3D dV4 = v4 - v2;
        Dir3D dV3 = v3 - v2;
        Dir3D dV1 = v1 - v2;

        // front
        addPlane(v1, v2, v4, v1 + dV4, material);

        // top
        addPlane(v1, v2, v3, v1 + dV3, material);

        // right
        addPlane(v4, v2, v3, v3 + dV4, material);

        // back
        addPlane(v3, v3 + dV4, v3 + dV4 + dV1, v3 + dV1, material);

        // bottom
        addPlane(v4, v4 + dV3, v4 + dV3 + dV1, v4 + dV1, material);

        // left
        addPlane(v1, v1 + dV3, v1 + dV3 + dV4, v1 + dV4, material);
    }

    void addPlane(Point3D p1, Point3D p2, Point3D p3, Point3D p4, Material material) {
        const Primitive::Triangle tr1 = {p1, p2, p3, material};
        const Primitive::Triangle tr2 = {p1, p3, p4, material};
        triangles.push_back(tr1);
        triangles.push_back(tr2);
    }

    void addNormalTriangle(Point3D p1, Point3D p2, Point3D p3, Dir3D n1, Dir3D n2, Dir3D n3, Material material) {
        const Primitive::Triangle tr = {p1, p2, p3, n1, n2, n3, material};
        triangles.push_back(tr);
    }
};

 
