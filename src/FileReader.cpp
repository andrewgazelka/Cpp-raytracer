
#include <fstream>
#include <sstream>
#include <PGA_3D.h>
#include "FileReader.h"

using std::ifstream;

FileInfo &FileReader::readFile(const string &fileName) {

    ifstream input("dasd");

    FileInfo fileInfo;

    std::string line;
    while (std::getline(input, line)) {

        std::istringstream lineStream;
        lineStream.str(line);

        std::string start;

        if (!(lineStream >> start)) { // a blank line
            continue;
        }

        if (start[0] == '#') { // if is comment
            continue;
        }

        if (start.back() != ':') {
            printf("Error entered a line without a command!\n");
            exit(1);
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
}
