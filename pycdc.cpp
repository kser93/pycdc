#include <cstring>
#include <filesystem>
#include <iostream>
#include "ASTree.h"

using std::cerr;
using std::cout;
using std::endl;
using std::filesystem::path;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "No input file specified" << endl;
        return 1;
    }
    path filepath{ argv[1] };
    PycModule mod;
    try {
        mod.loadFromFile(filepath.string().c_str());
    } catch (std::exception& ex) {
        cerr << "Error loading file " << filepath << ": " << ex.what() << endl;
        return 1;
    }
    if (!mod.isValid()) {
        cerr << "Could not load file " << filepath << endl;
        return 1;
    }
    cout << "# Source Generated with Decompyle++" << endl;
    cout << "# File: " << filepath.filename() << " (Python " << mod.getVersionString() << ")" << endl << endl;
    try {
        decompyle(mod.code(), &mod);
    } catch (std::exception& ex) {
        cerr << "Error decompyling " << filepath << ": " << ex.what() << endl;
        return 1;
    }

    return 0;
}
