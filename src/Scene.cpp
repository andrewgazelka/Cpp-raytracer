
#include "Scene.h"

#include <limits>

Color Scene::SpecularContribution(const Light *light, const Ray &viewRay, const HitInformation &hit) const {
    let mat = hit.material;
    let I_L = light->getIntensity(hit.location);

    // directionToLight 0.665371954, 0.74121666,  0.0887577161
    const Dir3D directionToLight = -light->getDirection(hit.location);

    // directionToViewer -0.635590612, 0.0786042064, -0.768014252
    const Dir3D directionToViewer = -viewRay.direction;

    // normal -0.031293232, 0.842518985, -0.53775692
    let normal = hit.normal;

    // -0.700166106, 0.195558965, -0.686676084
    const Dir3D reflectionOfLight = ReflectPointingAway(directionToLight, normal);

    let reflectCos = std::max({0.0f, dot(reflectionOfLight, directionToViewer)});
    let phongCoeff = powf(reflectCos, mat.phong);


    let dSpecularity = (mat.spectral * I_L) * reflectCos * phongCoeff;
    return dSpecularity;
}

Color Scene::ApplyLightingModel(const Ray &ray, const HitInformation &hit, float epsilon, int depth) {

    let[hitLocation, normal, originX, originY, t, material, shape] = hit;

    Color contribution; // black

    for (const auto *light : inputData.getLights()) { // TODO: change to const auto

        const Dir3D directionToLight = -light->getDirection(hitLocation);
        const float distanceToLight = light->getDistance(hitLocation);

        Ray rayToLight = {
                .origin = hitLocation,
                .direction = directionToLight
        };

        HitInformation shadowHit;
        bool blocked = FindIntersection(rayToLight, originX, originY, &shadowHit, 0.01);


        if (blocked && shadowHit.t < distanceToLight) {
            continue; // continue onto next light
        }

        contribution += DiffuseContribution(light, hit);
        contribution += SpecularContribution(light, ray, hit);
    }

    Ray mirror = Reflect(ray, hitLocation, normal); // TODO: is this right
    contribution += material.spectral * EvaluateRayTree(mirror, depth, hit.originX, hit.originY); // reflection

    if (material.transmissive.isNotBlack()) {
        const Ray rayRefract = Refract(ray, material, hitLocation, normal);
        contribution += material.transmissive * EvaluateRayTree(rayRefract, depth, originX, originY);
    }

    contribution += material.ambient * inputData.ambientLight;
    return contribution;
}

bool Scene::FindIntersection(Ray ray, float originX, float originY, HitInformation *hitInformation, float epsilon) {


    float minT = std::numeric_limits<float>::max();
    const Sphere *minSphere = nullptr;

    for (const auto &sphere: inputData.spheres) {
        auto t = raySphereIntersect(ray, sphere, epsilon);
        if (t && t.value() < minT) {
            minT = t.value();
            minSphere = &sphere;
        }
    }

    const Primitive::Triangle *minTriangle = nullptr;


    for (auto &triangle: triangles) {
        auto t = triangle.rayPlaneIntersect(ray.origin, ray.direction, epsilon);
        if (t && t.value() < minT) {
            Point3D point3D = ray.origin + ray.direction * t.value();
            let barry = triangle.barycentric(point3D);
            if (barry.isCollision()) {
                minT = t.value();
                minTriangle = &triangle;
            }
        }
    }

    if (minTriangle != nullptr) {
        Point3D location = ray.origin + ray.direction * minT;
        let barry = minTriangle->barycentric(location);
        *hitInformation = {
                .location = location,
                .normal = minTriangle->normalAt(barry, ray.direction),
                .originX = originX,
                .originY = originY,
                .t = minT,
                .material = minTriangle->material,
                .shape = TRIANGLE,
        };

        return true;
    } else if (minSphere != nullptr) {
        const Point3D hitLocation = ray.origin + ray.direction * minT;
        const Dir3D dirFromCenter = hitLocation - minSphere->center;
        const Dir3D normalUnnormalized = hitLocation - minSphere->center;

        const Dir3D normalNormalized = normalUnnormalized.normalized();

        *hitInformation = {
                .location = hitLocation,
                .normal = normalNormalized,
                .originX = originX,
                .originY = originY,
                .t = minT,
                .material = inputData.materials[minSphere->materialId],
                .shape = SPHERE
        };

        return true;
    }
    return false;
}

Color Scene::EvaluateRayTree(const Ray &ray, int depth, float originX, float originY) {

    HitInformation hit;

    if (depth <= inputData.maxDepth && FindIntersection(ray, originX, originY, &hit, 0.01)) {
        return ApplyLightingModel(ray, hit, 0.001, depth + 1);
    } else {
        return inputData.background;
    }

}

[[nodiscard]] std::optional<float>
Scene::raySphereIntersect(const Ray &ray, const Sphere &sphere, float epsilon) const {

    Point3D rayStart = ray.origin;
    Dir3D dir = ray.direction;
    const auto sphereCenter = sphere.center;
    const auto sphereRadius = sphere.radius;
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

Color Scene::DiffuseContribution(const Light *light, const HitInformation &hit) {

    let mat = hit.material;
    let normal = hit.normal;

    let I_L = light->getIntensity(hit.location);
    let directionToLight = -light->getDirection(hit.location);

    let lightCos = std::max({0.0f, dot(normal, directionToLight)}); // TODO: -1 needed

    return (mat.diffuse * I_L) * lightCos;
}

/**
 * Reflects ASSUMES that direction points into normal
 * @param direction
 * @param normal
 * @return
 */
Dir3D Scene::Reflect(const Dir3D &direction, const Dir3D &normal) const {
    const Dir3D reflection = direction - 2 * (dot(direction, normal) * normal);
    return reflection;
}

/**
 * Reflects ASSUMES that the two vectors (normal and direction) are both pointing AWAY from each other...
 * @param direction
 * @param normal
 * @return
 */
Dir3D Scene::ReflectPointingAway(const Dir3D &direction, const Dir3D &normal) const {
    const Dir3D reflection = 2 * (dot(direction, normal) * normal) - direction;
    return reflection;
}

Ray Scene::Refract(const Ray &ray, const Material &material, const Point3D &newOrigin, const Dir3D &normal) const {

    float dDotN = dot(ray.direction, normal);
    float dDotN2 = dDotN * dDotN;
    float eta = (dDotN < 0) ? 1.0f / material.ior : material.ior;
    float eta2 = eta * eta;

    // normal correctly oriented

    Dir3D n = (dDotN < 0) ? normal : -normal;
    dDotN = (dDotN > 0) ? -dDotN : dDotN;
    float disciminant = 1 - eta2 + dDotN2 * eta2;

    if (disciminant > 0) { // there is refraction
        // the sign is always negative since we oriented the normal correctly
        Dir3D refractDir = (-sqrtf(disciminant) - dDotN * eta) * n + eta * ray.direction;
        Ray rayRefract = Ray{
                .origin = newOrigin,
                .direction = refractDir.normalized()
        };
        return rayRefract;
    } else {
        return Reflect(ray, newOrigin, normal);
    }
}
