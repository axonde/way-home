#include "config.h"

Config::Config() {
    std::fstream config_file{config_name_, config_file.in};
    try {
        if (!config_file.is_open()) {
            Warnings::HintFirstCreateConfigFile();
            CreateConfigFile();
        } else {
            if (!Warnings::ProceedAlreadyCreatedConfig())
                CreateConfigFile();
        }
    } catch (const Errors::CantOpenFile&) {
        Warnings::ErrorWhileParsingConfig(); throw;
    } catch(...) {
        Warnings::ErrorAbnormal(); throw;
    }

    // already have working json file
}

void Config::CreateConfigFile() {
    std::fstream config_file{config_name_, config_file.out};
    
    if (!config_file.is_open()) {
        throw Errors::CantOpenFile{};
    }

    std::map<std::string, std::string> data;
    Warnings::HintCorrectnessCityNaming();
    std::cout << "Enter the departure city: ";
    std::cin >> data["from"];
    std::cout << "Enter the arrival city: ";
    std::cin >> data["to"];
    std::cout << "Enter the api key from Yandex Rapsisanie: ";
    std::cin >> data["api"];

    json data_json{data};
    config_file << std::setw(2) << data_json;
}