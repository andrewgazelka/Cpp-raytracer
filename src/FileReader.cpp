
#include <fstream>
#include <sstream>
#include "FileReader.h"
#include "commands.h"
#include <boost/algorithm/string.hpp>

using std::ifstream;
using boost::algorithm::to_lower;


int i = 1;


InputData FileReader::readFile(const string &fileName) {

    ifstream inputStream("dasd");

    InputData data;

    std::string line;
    uint materialId = 0;

    while (std::getline(inputStream, line)) {

        std::istringstream ls;
        ls.str(line);

        std::string start;


        if (!(ls >> start)) { // a blank line
            continue;
        }

        if (start[0] == '#') { // if is comment
            continue;
        }

        if (start.back() != ':') {
            printf("Error entered a line without a command!\n");
            exit(1);
        }

        to_lower(start); // makes sure lowercase

        Command command = Commands.get(start);

        switch (command) {

            case Command::INVALID:
                printf("the command %s is invalid!\n", start.c_str());
                exit(1);
            case Command::CAMERA_POS:
                ls >> data.cameraPos;
                break;
            case Command::CAMERA_FWD:
                ls >> data.cameraFwd;
                break;
            case Command::CAMERA_UP:
                ls >> data.cameraUp;
                break;
            case Command::CAMERA_FOV_HA:
                ls >> data.cameraFovHA;
                break;
            case Command::FILM_RESOLUTION:
                ls >> data.resolution;
                break;
            case Command::OUTPUT_IMAGE:
                ls >> data.outputImage;
                break;
            case Command::MAX_VERTICES:
                ls >> data.maxVertices;
                break;
            case Command::MAX_NORMALS:
                ls >> data.maxNormals;
                break;
            case Command::VERTEX: {
                Point3D toAdd;
                ls >> toAdd;
                data.vertices.push_back(toAdd);
                break;
            }
            case Command::NORMAL: {
                Dir3D toAdd;
                ls >> toAdd;
                data.normals.push_back(toAdd);
                break;
            }
            case Command::TRIANGLE: {
                Triangle toAdd;
                ls >> toAdd;
                toAdd.materialId = materialId;
                data.triangles.push_back(toAdd);
                break;
            }
            case Command::NORMAL_TRIANGLE: {
                NormalTriangle toAdd;
                ls >> toAdd;
                toAdd.materialId = materialId;
                data.normalTriangles.push_back(toAdd);
                break;
            }
            case Command::SPHERE: {
                Sphere toAdd;
                ls >> toAdd;
                toAdd.materialId = materialId;
                data.spheres.push_back(toAdd);
                break;
            }
            case Command::BACKGROUND:
                ls >> data.background;
                break;
            case Command::MATERIAL: {
                Material mat;
                ls >> mat;
                data.materials.push_back(mat);
                materialId++;
                break;
            }
            case Command::DIRECTIONAL_LIGHT: {
                DirectionalLight dl;
                ls >> dl;
                data.directionalLights.push_back(dl);
                break;
            }
            case Command::POINT_LIGHT: {
                PointLight pl;
                ls >> pl;
                data.pointLights.push_back(pl);
                break;
            }
            case Command::SPOT_LIGHT: {
                SpotLight sl;
                ls >> sl;
                data.spotLights.push_back(sl);
                break;
            }
            case Command::AMBIENT_LIGHT:
                ls >> data.ambientLight;
                break;
            case Command::MAX_DEPTH:
                ls >> data.maxDepth;
                break;
        }

    }
}


int main() {
    printf("hi");
}