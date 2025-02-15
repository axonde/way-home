#include "utils.h"

bool Warnings::ProceedAlreadyCreatedConfig() {
    std::cout << "Do you want to proceed with the already created config?\n";
    std::cout << "Type the answer (be careful: other answer except `yes` will be consider like `no`) (yes/no): ";
    char answer[4];
    std::cin >> answer;
    return strcmp(answer, "yes") == 0;
}

void Warnings::HintCorrectnessCityNaming() {
    std::cout << "Please remember that the city names must to be correct (all names are in russian yet)\n";
}
void Warnings::HintFirstCreateConfigFile() {
    std::cout << "Please provide some information about your trip.\n";
}

void Warnings::ErrorWhileParsingConfig() {
    std::cerr << "Error while parsing config file. Please make sure that the program have privileges for creating and reading the `wayhome-config.json` file.\n";
}
void Warnings::ErrorAbnormal() {
    std::cerr << "Happened a abnormal error. Can't help you, maybe retry?\n";
}
