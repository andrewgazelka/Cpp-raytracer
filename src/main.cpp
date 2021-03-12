//CSCI 5607 HW3 - Rays & Files
//This HW has three steps:
// 1. Compile and run the program (the program takes a single command line argument)
// 2. Understand the code in this file (rayTrace_pga.cpp), in particular be sure to understand:
//     -How ray-sphere intersection works
//     -How the rays are being generated
//     -The pipeline from rays, to intersection, to pixel color
//    After you finish this step, and understand the math, take the HW quiz on canvas
// 3. Update the file parse_pga.h so that the function parseSceneFile() reads the passed in file
//     and sets the relevant global variables for the rest of the code to product to correct image

//To Compile: g++ -fsanitize=address -std=c++11 rayTrace_pga.cpp

//For Visual Studios
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

//Images Lib includes:
#define STB_IMAGE_IMPLEMENTATION //only place once in one .cpp file
#define STB_IMAGE_WRITE_IMPLEMENTATION //only place once in one .cpp files

#include "image_lib.h" //Defines an image class and a color class

//#3D PGA
#include "PGA_3D.h"

//High resolution timer
#include <chrono>

#include "FileReader.h"
#include "Scene.h"

#include <optional>

using std::cout;
using std::endl;


std::optional<Point3D>
raySphereIntersect(Point3D rayStart, Line3D rayLine, Point3D sphereCenter, float sphereRadius, float epsilon = 0.01) {
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

            const auto dPos = dir * minT;
            const auto hitPos = rayStart + dPos;

            return hitPos;
        }
    }
    return {};
}

//
//bool raySphereIntersect(Point3D rayStart, Line3D rayLine, Point3D sphereCenter, float sphereRadius) {
//    Point3D projPoint = dot(rayLine, sphereCenter) *
//                        rayLine;      //Project to find closest point between circle center and line [proj(sphereCenter,rayLine);]
//    float distSqr = projPoint.distToSqr(sphereCenter);          //Point-line distance (squared)
//    float d2 = distSqr / (sphereRadius * sphereRadius);             //If distance is larger than radius, then...
//    if (d2 > 1) return false;                                   //... the ray missed the sphere
//    float w = sphereRadius * sqrt(1 -
//                                  d2);                          //Pythagorean theorem to determine dist between proj point and intersection points
//    Point3D p1 = projPoint + rayLine.dir() * w;                   //Add/subtract above distance to find hit points
//    Point3D p2 = projPoint - rayLine.dir() * w;
//
//    if (dot((p1 - rayStart), rayLine.dir()) >= 0)
//        return true;     //Is the first point in same direction as the ray line?
//    if (dot((p2 - rayStart), rayLine.dir()) >= 0)
//        return true;     //Is the second point in same direction as the ray line?
//    return false;
//}

/*
 * Point light
 * I_L = I_0 / (k_c + k_1d + k_q d^2)
 */

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Please provide the file you would like to process!" << endl;
        exit(1);
    }
    auto fileName = std::string(argv[1]);
    cout << "Parsing " << fileName << endl;
    const auto data = FileReader::readFile(fileName);

    const auto[imageWidth, imageHeight] = data.resolution;
    auto eye = data.cameraPos;
    auto forward = data.cameraForward;
    auto up = data.cameraUp;
    auto right = data.cameraRight;
    auto sphere = data.spheres[0];
    auto spherePos = sphere.pos;

    auto sphereRadius = sphere.radius;
    auto imgName = data.outputImage;

    // float versions
    const float imgW = imageWidth, imgH = imageHeight;

    const float halfW = (float) imageWidth / 2.0f, halfH = (float) imageHeight / 2.0f;
    const float halfAngleVFOV = data.cameraFovHA;
    const float d = halfH / tanf(halfAngleVFOV * (float) (M_PI / 180.0f));

    Image outputImg = Image(imageWidth, imageHeight);

    auto scene = Scene(data);
    auto t_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < imageWidth; i++) {
        for (int j = 0; j < imageHeight; j++) {
            float u = (halfW - (imgW) * (((float) i + 0.5f) / imgW));
            float v = (halfH - (imgH) * (((float) j + 0.5f) / imgH));
            Point3D p = eye - d * forward + u * right + v * up;
            Dir3D rayDir = (p - eye);
            Line3D rayLine = vee(eye, rayDir).normalized();

            auto hit = raySphereIntersect(eye, rayLine, spherePos, sphereRadius);
            if (hit) {
                const auto pointHit = hit.value();
                const auto lighting = scene.pointLightingOf(sphere, pointHit, rayDir);
                outputImg.setPixel(i, j, lighting);
            }

//            outputImg.setPixel(i, j, color);
            //outputImg.setPixel(i,j, Color(fabs(i/imgW),fabs(j/imgH),fabs(0))); //TODO: Try this, what is it visualizing?
        }
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    printf("Rendering took %.2f ms\n", std::chrono::duration<double, std::milli>(t_end - t_start).count());
//
    outputImg.write(imgName.c_str());
}
