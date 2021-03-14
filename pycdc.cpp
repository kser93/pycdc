#include <cstring>
#include <exception>
#include <filesystem>
#include <iostream>
#include "ASTree.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "No input file specified" << std::endl;
        return 1;
    }
    std::filesystem::path filepath{ argv[1] };

    try {
        PycModule mod(filepath);
        std::cout << "# Source Generated with Decompyle++" << std::endl;
        std::cout << "# File: " << filepath.filename() << " (Python " << mod.getVersionString() << ")" << std::endl << std::endl;
        decompyle(mod.code(), &mod);
    }
    catch (std::exception& ex) {
        std::cerr << "Error processing file " << filepath.string() << ":" << std::endl << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
