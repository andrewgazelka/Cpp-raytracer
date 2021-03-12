#pragma once


#include "InputData.h"

#define Kc 1 // standard term
#define K1 0.6 // proportional term
#define Kq 0.5 // squared term

class Scene {

public:
    explicit Scene(const InputData &inputData);

    /**
     * Preconditions: viewDirection is normalized
     *
     * @param material
     * @param location
     * @param viewDirection
     * @return
     */
    Color pointLightingOf(MaterialId material, Point3D location, Dir3D viewDirection) {

        const auto mat = inputData.materials[material.materialId];
        auto diffuseCoefficient = mat.diffuse;


        Color dColor;

        for (auto pointLight : inputData.pointLights) {

            const auto dVec = location - pointLight.location;
            const auto d = dVec.magnitude();
            const auto directionNormalized = dVec * (1 / d);
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

private:
    const InputData inputData;
};

 
