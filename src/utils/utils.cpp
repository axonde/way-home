#include "utils.h"

bool Warnings::ProceedAlreadyCreatedConfig() {
    std::cout << "Do you want to proceed with the already created config?\n";
    std::cout << "Type the answer (be careful: other answer except `yes` will be consider like `no`) (yes/no): ";
    std::string answer;
    std::getline(std::cin, answer);
    return answer == "yes";
}

void Warnings::HintCorrectnessCityNaming() {
    std::cout << "Please remember that the city names must to be correct (all names are in russian yet)\n";
}
void Warnings::HintFirstCreateConfigFile() {
    std::cout << "Please provide some information about your trip.\n";
}
void Warnings::HintNotFoundWays() {
    std::cout << "We have not found any way, however we have found another gem in this universe - you!\n";
}

void Warnings::ErrorWhileParsingConfig() {
    std::cerr << "Error while parsing config file. Please make sure that the program have privileges for creating and reading the `wayhome-config.json` file.\n";
}
void Warnings::ErrorAbnormal() {
    std::cerr << "Happened a abnormal error. Can't help you, maybe retry?\n";
}
void Warnings::ErrorBadConfig() {
    std::cerr << "Your config is ill. Please delete or override it.\n";
}
void Warnings::ErrorFewArgs() {
    std::cerr << "Very few arguments.. catastrophically little. So, goodbye.\n";
}
void Warnings::ErrorWhileFindRoutes() {
    std::cerr << "An error happened when i try to found routes. To be honest, it's the worst and simplest error that you can call.\n";
}
void Warnings::ErrorIllJsonAnswer() {
    std::cerr << "A route was skipped due to ill formating.\n";
}

std::string Errors::CantOpenFile::what() const {
    return "Can't open the file( or can, but don't want.";
}
std::string Errors::BadJson::what() const {
    return "Your JSON is ill-formatted. Please cure him (or retry with overriding)!";
}
std::string Errors::ErrorParseJson::what() const {
    return "An error occured while i parsed the JSON file.. How you feel now?";
}
std::string Errors::ErrorInvalidDate::what() const {
    return "Date format is strict YYYY-MM-DD.";
}
std::string Errors::ErrorRequestBadStatus::what() const {
    return "Unforunately, the API return a very bad word. Status code: " + std::to_string(status_code_);
}
std::string Errors::ErrorApiCode::what() const {
    return "Yandex API haven't indexed your city yet. Hahah.. ha.. :0";
}
std::string Errors::ErrorApiCityNotFound::what() const {
    return "Didn't find your city(" + city_ + "). Maybe try again? m?";
}
