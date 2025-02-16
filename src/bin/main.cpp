#include <iostream>
#include <fstream>
#include <memory>
#include <config/config.h>
#include <utils/utils.h>
#include <finder/finder.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        Warnings::ErrorFewArgs();
        return 1;
    }

    std::unique_ptr<Config> config;
    try {
        config = std::make_unique<Config>(argv[1]);
    } catch (const Errors::Error& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    Finder finder(config.get());
    if (!finder.InitializeData()) {
        return 1;
    }

    if (!finder.SetYandexCodes()) {
        return 1;
    }

    


    return 0;
}