
//Set the global scene parameter variables
//TODO: Set the scene parameters based on the values in the scene file

#ifndef PARSE_PGA_H
#define PARSE_PGA_H

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

using std::ifstream;
using std::ios;
using std::cout;
using std::endl;
//Camera & Scene Parmaters (Global Variables)
//Here we set default values, override them in parseSceneFile()

//Image Parmaters
int img_width = 800, img_height = 600;
std::string imgName = "raytraced.png";

//Camera Parmaters
Point3D eye = Point3D(0, 0, 0);
Dir3D forward = Dir3D(0, 0, -1).normalized();
Dir3D up = Dir3D(0, 1, 0).normalized();
Dir3D right = Dir3D(-1, 0, 0).normalized();
float halfAngleVFOV = 35;

//Scene (Sphere) Parmaters
Point3D spherePos = Point3D(0, 0, 2);
float sphereRadius = 1;

void parseSceneFile(std::string fileName) {
    ifstream input(fileName);

    std::string line;
    while (std::getline(input, line)) {

        std::istringstream lineStream;
        lineStream.str(line);

        std::string start;
        lineStream >> start;

        if (start[0] == '#') { // if is comment
            continue;
        }

        if (start == "sphere:") { // ✅
            float x, y, z, r;
            lineStream >> x >> y >> z >> r;
            spherePos = Point3D(x, y, z);
            sphereRadius = r; // TODO: test when no r provided
        } else if (start == "image_resolution:") { // ✅
            lineStream >> img_width >> img_height;
        } else if (start == "output_file:") { // ✅
            lineStream >> imgName;
        } else if (start == "camera_pos:") { // ✅
            float x, y, z;
            lineStream >> x >> y >> z;
            eye = Point3D(x, y, z);
        } else if (start == "camera_fwd:") { // ✅
            float fx, fy, fz;
            lineStream >> fx >> fy >> fz;
            forward = Dir3D(fx, fy, fz).normalized();
        } else if (start == "camera_up:") {// ✅
            float ux, uy, uz;
            lineStream >> ux >> uy >> uz;
            up = Dir3D(ux, uy, uz).normalized();
        } else if (start == "camera_fov_ha:") { // ✅
            lineStream >> halfAngleVFOV;
        }

    }

    //TODO: Create an orthogonal camera basis, based on the provided up and right vectors
    printf("Orthagonal Camera Basis:\n");
    right = cross(up, forward);
    if (right.magnitudeSqr() == 0.0) {
        printf("forward and up are parallel! Exiting.\n");
        exit(1);
    }

    right = right.normalized();

    cout << "output file: " << imgName << endl;
    cout << "image resolution: " << img_width << " " << img_height << endl;
    cout << "sphere: " << spherePos.x << " " << spherePos.y << " " << spherePos.z << " " << sphereRadius << endl;
    cout << "camera_pos: " << eye.x << " " << eye.y << " " << eye.z << endl;
    cout << "camera_forward: " << forward.x << " " << forward.y << " " << forward.z << endl;
    cout << "camera_up: " << up.x << " " << up.y << " " << up.z << endl;
    cout << "camera_fov: " << halfAngleVFOV << endl;
    cout << "camera_right: " << right.x << " " << right.y << " " << right.z << endl;
}

#endif