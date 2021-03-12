#pragma once


#include "InputData.h"
#include <optional>

#define Kc 1.0f // standard term
#define K1 0.6f // proportional term
#define Kq 0.5f // squared term

using std::vector;

class Scene {

public:
    explicit Scene(const InputData &inputData);

    /**
     * Preconditions: viewDirection is normalized
     *
     * @param material
     * @param hitLocation
     * @param viewDirection
     * @return
     */
    [[nodiscard]] Color pointLightingOf(const MaterialId &material, const Point3D &hitLocation, const Dir3D &viewDirection,
                                        const vector<Sphere> &spheres) {

        const auto mat = inputData.materials[material.materialId];
        auto diffuseCoefficient = mat.diffuse;


        Color dColor;

        for (auto pointLight : inputData.pointLights) {

            const Dir3D dVec = pointLight.location - hitLocation;
            const auto d = dVec.magnitude();
            const auto directionNormalized = dVec * (1 / d);

            const auto rayLine = vee(hitLocation, directionNormalized).normalized();

            // continue because there is an intersection
            if (anyIntersect(hitLocation, rayLine, spheres)) continue;

            const auto angle = dot(directionNormalized, viewDirection);

            for (char i = 0; i < CHANNELS; ++i) {

                auto coeff = diffuseCoefficient[i];
                const auto initialIntensity = pointLight.color[i];


                const auto loseScalar = (Kc + K1 * d + Kq * d * d);
                const auto I_L = initialIntensity / loseScalar;

                dColor[i] += coeff * I_L;
            }
        }
        return dColor;
    }

    bool anyIntersect(Point3D rayStart, Line3D rayLine, const vector<Sphere> &spheres, float epsilon = 0.01) {
        for (const auto &sphere: spheres) {
            const auto interesect = raySphereIntersect(rayStart, rayLine, sphere, epsilon);
            if (interesect) {
                return true;
            }
        }
        return false;
    }

    std::optional<float>
    raySphereIntersect(Point3D rayStart, Line3D rayLine, const Sphere &sphere, float epsilon = 0.01) const {
        const auto sphereCenter = sphere.pos;
        const auto sphereRadius = sphere.radius;
        Dir3D dir = rayLine.dir();
        float a = dot(dir, dir);
        Dir3D toStart = (rayStart - sphereCenter);
        float b = 2 * dot(dir, toStart);
        float c = dot(toStart, toStart) - sphereRadius * sphereRadius;
        float disriminant = b * b - 4 * a * c;
        if (disriminant < 0) return {};
        else {
            float t0 = (-b + sqrt(disriminant)) / (2 * a);
            float t1 = (-b - sqrt(disriminant)) / (2 * a);
            if (t0 > epsilon || t1 > epsilon) {
                float minT;
                if (t0 <= epsilon) {
                    minT = t1;
                } else if (t1 <= epsilon) {
                    minT = t0;
                } else {
                    minT = std::min({t0, t1});
                }

                return minT;
            }
        }
        return {};
    }

private:
    const InputData inputData;
};

 
