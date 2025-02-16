#include "config.h"

Config::Config(const std::string& date) : date_(date) {
    if (!IsValidDate(date)) {
        throw Errors::ErrorInvalidDate();
    }

    std::fstream config_file(config_name_, config_file.in);
    try {
        if (!config_file.is_open()) {
            Warnings::HintFirstCreateConfigFile();
            CreateConfigFile();
        } else {
            if (!Warnings::ProceedAlreadyCreatedConfig())
                CreateConfigFile();
        }
    } catch (...) {
        throw;
    }

    std::stringstream buffer;
    buffer << config_file.rdbuf();

    json config_json;
    try {
        config_json = json::parse(buffer.str());
    } catch (...) {
        throw Errors::ErrorParseJson();
    }
    
    if (!CheckJson(config_json)) {
        throw Errors::BadJson();
    }
    api_key_ = config_json["api"];
    from_ = config_json["from"];
    to_ = config_json["to"];
}

std::map<std::string, std::string> Config::GetParameters() const {
    return {
        {"apikey", api_key_},
        {"from", from_},
        {"to", to_},
    };
}

void Config::CreateConfigFile() {
    std::fstream config_file(config_name_, config_file.out);
    
    if (!config_file.is_open()) {
        throw Errors::CantOpenFile();
    }

    std::map<std::string, std::string> data;
    Warnings::HintCorrectnessCityNaming();
    std::cout << "Enter the departure city: ";
    std::cin >> data["from"];
    std::cout << "Enter the arrival city: ";
    std::cin >> data["to"];
    std::cout << "Enter the api key from Yandex Raspisanie: ";
    std::cin >> data["api"];

    json data_json(data);
    config_file << std::setw(2) << data_json;
}

bool Config::CheckJson(const json& data) {
    std::list<std::string> parameters = {"from", "to", "api"};
    return std::all_of(parameters.begin(), parameters.end(), [&](const auto& param) {
        return data.find(param) != data.end();
    });
}

bool Config::IsValidDate(const std::string& date) {
    std::regex date_regex(R"(^\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$)");
    return std::regex_match(date, date_regex);
}
