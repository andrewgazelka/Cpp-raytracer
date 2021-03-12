#pragma once


#include "InputData.h"
#include "utils.h"
#include <optional>

#define Kc 1.0f // standard term
#define K1 0.6f // proportional term
#define Kq 0.5f // squared term

using std::vector;

class Scene {

public:
    explicit Scene(const InputData &inputData);

    Color lightingOf(const Sphere &sphere, const Point3D &hitLocation, const Dir3D &viewDirection,
                     const vector<Sphere> &spheres) {
        let pointLighting = lightingOfSphere(sphere, hitLocation, viewDirection, spheres);
        let ambientLighting = inputData.ambientLight;
        return pointLighting + ambientLighting;
    }

    [[nodiscard]] bool
    anyIntersect(Point3D rayStart, Line3D rayLine, const vector<Sphere> &spheres, float epsilon = 0.01) const {
        for (const auto &sphere: spheres) {
            const auto interesect = raySphereIntersect(rayStart, rayLine, sphere, epsilon);
            if (interesect) {
                return true;
            }
        }
        return false;
    }


    [[nodiscard]] std::optional<float>
    raySphereIntersect(Point3D rayStart, Line3D rayLine, const Sphere &sphere, float epsilon = 0.01) const {
        const auto sphereCenter = sphere.center;
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

    /**
     * Preconditions: viewDirection is normalized
     *
     * @param material
     * @param hitLocation
     * @param viewDirection
     * @return
     */
    [[nodiscard]] Color
    lightingOfSphere(const Sphere &sphere, const Point3D &hitLocation, const Dir3D &viewDirection,
                     const vector<Sphere> &spheres) {

        let material = inputData.materials[sphere.materialId];
        let sphereCenter = sphere.center;

        // normal to the sphere surface. It points into the sphere so angles are preserved
        const Dir3D normalUnnormalized = (sphereCenter - hitLocation);
        const Dir3D normal = normalUnnormalized.normalized();

        Color dColor;

        for (auto pointLight : inputData.pointLights) { // TODO: change to const auto

            let dLight = pointLight.location - hitLocation;
            let distanceToLight = dLight.magnitude();

            let lightDirection = dLight * (1 / distanceToLight);

            // reflection of light over normal
            const Dir3D reflectionOfLight = lightDirection - 2 * (dot(lightDirection, normal) * normal);

            let rayLine = vee(hitLocation, lightDirection).normalized();

            // continue because there is an intersection
            if (anyIntersect(hitLocation, rayLine, spheres)) continue;


            let reflectCos = std::max({0.0f, dot(reflectionOfLight, viewDirection)});
            let lightCos = std::max({0.0f, dot(normal * (-1), lightDirection)});
            let phongCoeff = powf(reflectCos, material.phong);

            for (char i = 0; i < CHANNELS; ++i) {

                // diffuse coefficient
                let kD = material.diffuse[i];
                let kS = material.spectral[i];

                let lightIntensity = pointLight.color[i];
                let loseScalar = (Kc + K1 * distanceToLight + Kq * distanceToLight * distanceToLight);

                // intensity of the light at that point
                let I_L = lightIntensity / loseScalar;

                let dDiffuse = kD * I_L * lightCos;
                let dSpecularity = kS * reflectCos * phongCoeff * I_L;

                dColor[i] += (dDiffuse + dSpecularity);
            }
        }
        return dColor;
    }
};

 
