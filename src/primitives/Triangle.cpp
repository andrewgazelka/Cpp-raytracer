
#include <utils.h>
#include "Triangle.h"

namespace Primitive {
    Triangle::Triangle(const Point3D &p1, const Point3D &p2, const Point3D &p3, const Material &material) : p1(p1),
                                                                                                            p2(p2),
                                                                                                            p3(p3),
                                                                                                            material(
                                                                                                                    material) {

        // the two vectors that span the triangle plane
        Dir3D v1 = p2 - p1; // alpha
        Dir3D v2 = p3 - p1; // beta

        // 0 <= alpha <= 1
        // 0 <= beta <= 1
        // alpha + beta <= 1


        const Dir3D normal = cross(v1, v2).normalized();

        // we will still interpolate but between three
        n1 = normal, n2 = normal, n3 = normal;
    }


    Triangle::Triangle(const Point3D &p1, const Point3D &p2, const Point3D &p3, const Dir3D &n1, const Dir3D &n2,
                       const Dir3D &n3, const Material &material) : p1(p1), p2(p2), p3(p3), n1(n1), n2(n2), n3(n3),
                                                                    material(material) {}


    std::optional<float> Triangle::rayPlaneIntersect(const Point3D &eye, const Dir3D &dir, float epsilon) const {

        /*
         * First step: find the intersection with the plane
         */
        let denominator = dot(norm, dir);

        // the ray is parallel to the plane. It will never collide
        if (abs(denominator) < epsilon) return {};

        /*
         *  for a point in the plane [c] all points [p] on the plane  dot([p] - [c], [norm]) must be zero
         *  let us consider p1 the arbitrary point [c]
         *
         *  [p] = RAY_ORIGIN + RAY_DIRECTION * t
         *  [c] = p1
         *
         *  dot(p1 - (RAY_ORIGIN + RAY_DIRECTION * t), [norm]) = 0
         *  dot(p1, [norm]) - dot(RAY_ORIGIN, [norm]) - t * dot(RAY_DIRECTION, [norm]) = 0
         *  t * dot(RAY_DIRECTION, [norm]) = dot(p1 - RAY_ORIGIN, [norm])
         *  t = dot(p1 - RAY_ORIGIN, [norm]) / dot(RAY_DIRECTION, [norm])
         *  t = dot(p1 - RAY_ORIGIN, [norm]) / denominator
         *
         */

        float t = dot(static_cast<Dir3D>(p1 - eye), norm) / denominator;

        // the collision is in the opposite direction
        if (t < -epsilon) return {};

        return t;
    }

    Barycentric Triangle::barycentric(const Point3D &point) const {

        // inspiration from https://gamedev.stackexchange.com/a/23745

        Dir3D v0 = p2 - p1, v1 = p3 - p1, v2 = point - p1;

        float d00 = dot(v0, v0);
        float d01 = dot(v0, v1);
        float d11 = dot(v1, v1);
        float d20 = dot(v2, v0);
        float d21 = dot(v2, v1);

        float denom = d00 * d11 - d01 * d01;

        float alpha = (d11 * d20 - d01 * d21) / denom;
        float beta = (d00 * d21 - d01 * d20) / denom;
        float gamma = 1.0f - alpha - beta;

        return {alpha, beta, gamma};
    }

    Dir3D Triangle::normalAt(Barycentric barycentric) const {
        const auto[alpha, beta, gamma] = barycentric;
        const Dir3D sum = gamma * n1 + alpha * n2 + beta * n3;
        const Dir3D avg = sum / 3.0f;
        return avg;
    }


}