
#include <fstream>
#include <sstream>
#include "FileReader.h"
#include "commands.h"
#include <boost/algorithm/string.hpp>

using std::ifstream;
using boost::algorithm::to_lower;


int i = 1;


InputData FileReader::readFile(const string &fileName) {

    ifstream input("dasd");

    InputData inputData;

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

        to_lower(start); // makes sure lowercase

        Command command = Commands.get(start);

        switch (command) {

            case Command::INVALID:
                printf("the command %s is invalid!\n", start.c_str());
                exit(1);
                break;
            case Command::CAMERA_POS:
                break;
            case Command::CAMERA_FWD:
                break;
            case Command::CAMERA_UP:
                break;
            case Command::CAMERA_FOV_HA:
                break;
            case Command::FILM_RESOLUTION:
                break;
            case Command::OUTPUT_IMAGE:
                break;
            case Command::MAX_VERTICES:
                break;
            case Command::MAX_NORMALS:
                break;
            case Command::VERTEX:
                break;
            case Command::NORMAL:
                break;
            case Command::TRIANGLE:
                break;
            case Command::NORMAL_TRIANGLE:
                break;
            case Command::SPHERE:
                break;
            case Command::BACKGROUND:
                break;
            case Command::MATERIAL:
                break;
            case Command::NORMAL_TRIANGLE:
                break;
        }

    }
}


int main() {
    printf("hi");
}