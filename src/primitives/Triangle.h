#pragma once


#include <PGA_3D.h>
#include <optional>
#include <InputData.h>

struct Barycentric {
    float alpha, beta, gamma;


    [[nodiscard]] bool isCollision() const {
//        return true;
        return (0.0f <= alpha && alpha <= 1.0f) &&
               (0.0f <= beta && beta <= 1.0f) &&
               (0.0f <= gamma && gamma <= 1.0f);
    }

};

namespace Primitive {


    class Triangle {

    public:
        Triangle(const Point3D &p1, const Point3D &p2, const Point3D &p3, const Material &material);

        Triangle(const Point3D &p1, const Point3D &p2, const Point3D &p3, const Dir3D &n1, const Dir3D &n2,
                 const Dir3D &n3, const Material &material);

        [[nodiscard]] std::optional<float>
        rayPlaneIntersect(const Point3D &eye, const Dir3D &dir, float epsilon = 0.01);

        [[nodiscard]] Barycentric barycentric(const Point3D &point) const;

        [[nodiscard]] Dir3D normalAt(Barycentric barycentric, Dir3D rayDir) const;

        Material material;
    private:

        Point3D p1;
        Point3D p2;
        Point3D p3;

        Dir3D norm;

        Dir3D n1;
        Dir3D n2;
        Dir3D n3;
    };
};

 
