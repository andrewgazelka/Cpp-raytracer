#pragma once

#include <string>

using std::string;

struct Resolution {
    float width;
    float height;
};

struct FileInfo {
    Point3D cameraPos;
    Dir3D cameraFwd;
    Dir3D cameraUp;
    float cameraHA;
    Resolution resolution;
    string outputImage;
    int maxVertices;
    int maxNormals;

};

namespace FileReader {
    FileInfo &readFile(const string &fileName);
}

 
