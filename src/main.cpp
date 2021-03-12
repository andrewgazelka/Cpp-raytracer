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
    auto result = FileReader::readFile(fileName);
    printf("done\n");
}
