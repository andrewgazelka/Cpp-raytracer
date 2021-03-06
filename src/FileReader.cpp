
#include <fstream>
#include <sstream>
#include "FileReader.h"
#include "commands.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

using std::ifstream;
using boost::algorithm::to_lower;

InputData FileReader::readFile(const string &fileName) {

    const string &fn = std::filesystem::canonical(fileName);
    boost::filesystem::path p(fn);

    string outputFileName = p.stem().string(); // example if file is tester.txt will return .txt
    outputFileName.append(".png");

    ifstream inputStream(fn);


    std::cout << "Parsing " << fn << std::endl;

    // is empty
    if (inputStream.peek() == std::ifstream::traits_type::eof()) {
        printf("exit or non-existant file %s\n", fn.c_str());
        exit(1);
    }

    InputData data;

    // add default material
    Material defaultMat;
    data.materials.push_back(defaultMat);

    std::string line;
    uint materialId = 0;
    std::optional<Crop> crop;
    std::optional<string> name;

    // READ DATA
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
            printf("Error entered a line without a command! --- %s\n", line.c_str());
            exit(1);
        }

        // remove ":"
        start.pop_back();

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
                ls >> data.cameraForward;
                break;
            case Command::PLANE: {
                Plane toAdd;
                ls >> toAdd;
                toAdd.materialId = materialId;
                data.planes.push_back(toAdd);
                break;
            }
            case Command::BOX: {
                Box toAdd;
                ls >> toAdd;
                toAdd.materialId = materialId;
                data.boxes.push_back(toAdd);
                break;
            }
            case Command::CAMERA_UP:
                ls >> data.cameraUp;
                break;
            case Command::CAMERA_FOV_HA:
                ls >> data.cameraFovHA;
                break;
            case Command::FILM_RESOLUTION:
                ls >> data.resolution;
                break;
            case Command::CROP: {
                Crop cropTemp;
                ls >> cropTemp;
                crop = cropTemp;
                break;
            }
            case Command::OUTPUT_IMAGE: {
                string outputImage;
                ls >> outputImage;
                name = outputImage;
                break;
            }
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
            case Command::SAMPLES:
                ls >> data.samples;
                break;
        }

    }

    data.crop = crop ? crop.value() : Crop(0, 0, data.resolution.width, data.resolution.height);
    data.outputImage = name ? name.value() : outputFileName;

    // NORMALIZE DATA
    data.cameraUp = data.cameraUp.normalized();
    data.cameraForward = data.cameraForward.normalized();

    auto right = cross(data.cameraUp, data.cameraForward);

    if (right.magnitudeSqr() == 0.0) {
        printf("forward and up are parallel! Exiting.\n");
        printf("up was %f %f %f\n", data.cameraUp.x, data.cameraUp.y, data.cameraUp.z);
        printf("right was %f %f %f\n", data.cameraRight.x, data.cameraRight.y, data.cameraRight.z);
        exit(1);
    }

    data.cameraRight = right.normalized();

    return data;
}
