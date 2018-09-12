#include "file_loader.h"
#include <fstream>

namespace HiveEngineRenderer {
    std::string load_file(std::string location) {
        std::ifstream stream(location.c_str());
        if (!stream.is_open()) {
            return "";
        }
        std::string data = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
        stream.close();
        return data;
    }
}
