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
#include <limits>
#include <boost/random.hpp>

#define SEED 1234

using std::cout;
using std::endl;

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Please provide the file you would like to process!" << endl;
        exit(1);
    }


    boost::random::mt19937 gen(SEED);
    boost::random::uniform_real_distribution<float> dist(0.0, 1.0);
    dist(gen);

    let fileName = std::string(argv[1]);
    cout << "Parsing " << fileName << endl;
    let data = FileReader::readFile(fileName);

    let[imageWidth, imageHeight] = data.resolution;
    let eye = data.cameraPos;
    let forward = data.cameraForward;
    let up = data.cameraUp;
    let right = data.cameraRight;

    let imgName = data.outputImage;
    let samples = data.samples;

    // float versions
    let imgW = imageWidth, imgH = imageHeight;

    let halfW = (float) imageWidth / 2.0f, halfH = (float) imageHeight / 2.0f;
    let halfAngleVFOV = data.cameraFovHA;
    let d = halfH / tanf(halfAngleVFOV * (float) (M_PI / 180.0f));

    Image outputImg = Image(imageWidth, imageHeight);

    auto scene = Scene(data);
    auto t_start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < imageWidth; i++) {
        for (int j = 0; j < imageHeight; j++) {
            Color sampleSums;
            for (int s = 0; s < samples; s++) {

                // First sample is in the center. The rest are random
                let dx = s == 0 ? 0.5f : dist(gen);
                let dy = s == 0 ? 0.5f : dist(gen);

                let centerI = (float) i + dx;
                let centerJ = (float) j + dy;

                let u = (halfW - (imgW) * (centerI / imgW));
                let v = (halfH - (imgH) * (centerJ / imgH));

                Point3D p = eye - d * forward + u * right + v * up;
                Dir3D rayDir = (p - eye);
                Line3D rayLine = vee(eye, rayDir).normalized();

                std::optional<Sphere> closest = {};
                float closestT = std::numeric_limits<float>::max();
                for (const auto sphere: data.spheres) {
                    auto hit = scene.raySphereIntersect(eye, rayLine, sphere);
                    if (hit) {
                        auto value = hit.value();
                        if (value < closestT) {
                            closest = sphere; // TODO: passing is bad?
                            closestT = value;
                        }
                    }
                }
                if (closest) {
                    let sphere = closest.value();
                    let pointHit = eye + (rayLine.dir() * closestT);
                    let lighting = scene.lightingOf(sphere, pointHit, rayDir.normalized(), data.spheres);
                    sampleSums = sampleSums + lighting; // TODO: why can't this be +=
                } else {
                    sampleSums = sampleSums + data.background;
                }
            }
            let predictedColor = sampleSums / samples;
            outputImg.setPixel(i, j, predictedColor);
        }
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    printf("Rendering took %.2f ms\n", std::chrono::duration<double, std::milli>(t_end - t_start).count());
    outputImg.write(imgName.c_str());
}
