
#include "Scene.h"

#include <utility>
#include <limits>

Color Scene::SpecularContribution(const Light *light, const Ray &viewRay, const HitInformation &hit) {

    let viewDirection = viewRay.direction;

    let mat = hit.material;

    let lightDirection = light->getDirection(hit.location);
    let I_L = light->getIntensity(hit.location);

    let normal = hit.normal;

    const Dir3D reflectionOfLight = Reflect(lightDirection, normal);

    let reflectCos = std::max({0.0f, dot(reflectionOfLight, viewDirection)});
    let phongCoeff = powf(reflectCos, mat.phong);

    let dSpecularity = (mat.spectral * I_L) * reflectCos * phongCoeff;
    return dSpecularity;
}

Color Scene::ApplyLightingModel(Ray ray, HitInformation hit, float iorIn, float epsilon, int depth) {

    let[hitLocation, normal, t, material] = hit;

    Color contribution; // black

    for (const auto *light : inputData.getLights()) { // TODO: change to const auto

        let lightDirection = light->getDirection(hitLocation);
        let distanceToLight = light->getDistance(hitLocation);

        const Line3D rayLine = vee(hitLocation, lightDirection).normalized();
        let rayDir = rayLine.dir();

        Ray rayToLight = {
                .origin = hitLocation,
                .direction = rayDir
        };

        HitInformation shadowHit;
        bool blocked = FindIntersection(rayToLight, &shadowHit);


        if (blocked && shadowHit.t < distanceToLight - epsilon) continue; // continue onto next light

        contribution += DiffuseContribution(light, hit);
        contribution += SpecularContribution(light, ray, hit);
    }

    Ray mirror = Reflect(ray, hitLocation, normal); // TODO: is this right
    contribution += material.transmissive * EvaluateRayTree(mirror, iorIn); // reflection

    // tell if coming in or out of object
    float iorOut = iorIn == 1.0f ? material.ior : 1.0f;

    // todo glass
    Ray glass = Refract(ray, iorIn, iorOut, hitLocation, normal); // TODO: is this right
    contribution += material.transmissive * EvaluateRayTree(glass, iorOut); // TODO:


    contribution += material.ambient;

    // area light sources???

    return contribution;

}

bool Scene::FindIntersection(Ray ray, HitInformation *hitInformation) {

    float minT = std::numeric_limits<float>::max();
    const Sphere *minSphere = nullptr;

    for (const auto &sphere: inputData.spheres) {
        auto t = raySphereIntersect(ray, sphere, 0.01);
        if (t && t.value() < minT) {
            minT = t.value();
            minSphere = &sphere;
        }
    }

    const Primitive::Triangle *minTriangle = nullptr;


    for (const auto &triangle: triangles) {
        auto t = triangle.rayPlaneIntersect(ray.origin, ray.direction);
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
                .normal = minTriangle->normalAt(barry),
                .t = minT,
                .material = minTriangle->material,
        };

        return true;
    } else if (minSphere != nullptr) {
        Point3D location = ray.origin + ray.direction * minT;
        Dir3D norm = (ray.origin - minSphere->center).normalized();
        *hitInformation = {
                .location = location,
                .normal = norm,
                .t = minT,
                .material = inputData.materials[minSphere->materialId]
        };

        return true;
    }
    return false;
}

Color Scene::EvaluateRayTree(Ray ray, float iorIn, int depth) {

    HitInformation hit;

    if (depth <= inputData.maxDepth && FindIntersection(ray, &hit)) {
        return ApplyLightingModel(ray, hit, iorIn);
    } else {
        return inputData.background;
    }

}

[[nodiscard]] std::optional<float> Scene::raySphereIntersect(const Ray &ray, const Sphere &sphere, float epsilon) const {

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
    let lightDirection = light->getDirection(hit.location);

    let lightCos = std::max({0.0f, dot(normal * (-1), lightDirection)}); // TODO: -1 needed

    return (mat.diffuse * I_L) * lightCos;
}

Dir3D Scene::Reflect(const Dir3D &direction, const Dir3D &normal) const {
    const Dir3D reflection = direction - 2 * (dot(direction, normal) * normal);
    return reflection;
}

Ray Scene::Refract(const Ray &ray, float iorIn, float iorOut, const Point3D newOrigin, const Dir3D &normal) const {

    let d = ray.direction;

    let n = iorOut;
    let nt = iorIn;

    let n2 = n * n;
    let nt2 = nt * nt;

    let dDotN = dot(d, normal);
    let dDotN2 = dDotN * dDotN;

    const Dir3D first = n * (d - normal * dDotN) / nt;
    const Dir3D second = normal * sqrtf(1.0f - n2 * (1 - dDotN2) / nt2);

    const Dir3D res = first - second;
    return {newOrigin, res};
}
