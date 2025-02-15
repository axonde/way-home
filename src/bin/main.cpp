#include <iostream>
#include <fstream>
#include <config/config.h>
#include <utils/utils.h>
#include <finder/finder.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        Warnings::ErrorFewArgs();
        return 1;
    }

    try {
        Config config(argv[1]);
    } catch (const Errors::Error& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    Finder finder;

    return 0;
}