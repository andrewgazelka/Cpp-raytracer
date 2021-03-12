#include <cstdio>
#include <string>
#include <iostream>
#include "FileReader.h"

using std::cout;
using std::endl;

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Please provide the file you would like to process!" << endl;
        exit(1);
    }
    auto fileName = std::string(argv[1]);
    cout << "Parsing " << fileName << endl;
    auto data = FileReader::readFile(fileName);

    float imgW = img_width, imgH = img_height;
    float halfW = imgW/2, halfH = imgH/2;
    float d = halfH / tanf(halfAngleVFOV * (M_PI / 180.0f));

    Image outputImg = Image(img_width,img_height);
    auto t_start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < img_width; i++){
        for (int j = 0; j < img_height; j++){
            //TODO: In what way does this assumes the basis is orthonormal?
            float u = (halfW - (imgW)*((i+0.5)/imgW));
            float v = (halfH - (imgH)*((j+0.5)/imgH));
            Point3D p = eye - d*forward + u*right + v*up;
            Dir3D rayDir = (p - eye);
            Line3D rayLine = vee(eye,rayDir).normalized();  //Normalizing here is optional
            bool hit = raySphereIntersect(eye,rayLine,spherePos,sphereRadius);
            Color color;
            if (hit) color = Color(1,1,1);
            else color = Color(0,0,0);
            outputImg.setPixel(i,j, color);
            //outputImg.setPixel(i,j, Color(fabs(i/imgW),fabs(j/imgH),fabs(0))); //TODO: Try this, what is it visualizing?
        }
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    printf("Rendering took %.2f ms\n",std::chrono::duration<double, std::milli>(t_end-t_start).count());

    outputImg.write(imgName.c_str());
}
