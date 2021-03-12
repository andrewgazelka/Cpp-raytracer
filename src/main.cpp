#include <cstdio>
#include <string>
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char **argv) {
    printf("Parsing file");
    if (argc != 2) {
        cout << "Please provide the file you would like to process!" << endl;
        exit(1);
    }
    auto fileName = std::string(argv[1]);
    cout << "Parsing " << fileName << endl;
}
