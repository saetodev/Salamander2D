#include "Util.h"

std::string Util::ReadEntireFile(const std::string& filename) {
    std::fstream file(filename);

    if (!file.is_open()) {
        std::cout << "WARNING: Could not open file " << filename << std::endl;
        return "";
    }

    std::string line;
    std::stringstream stream;

    while (std::getline(file, line)) {
        stream << line << "\n";
    }

    return stream.str();
}