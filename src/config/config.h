#pragma once
#include <ctime>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <nlohmann/json.hpp>
#include <utils/utils.h>

using json = nlohmann::json;

class Config {
public:
    Config();
private:
    const std::string config_name_ = "wayhome-config.json";
    std::string api_key_;
    std::string from_;
    std::string to_;
    std::string date_;
    std::time_t curr_time_;

    void CreateConfigFile();
};
