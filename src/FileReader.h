#pragma once

#include <string>
#include <vector>
#include "InputData.h"

using std::string;

namespace FileReader {
    InputData readFile(const string &fileName);
}

 
