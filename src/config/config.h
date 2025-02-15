#pragma once
#include <algorithm>
#include <ctime>
#include <iostream>
#include <fstream>
#include <list>
#include <regex>
#include <stdexcept>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>
#include <utils/utils.h>

using json = nlohmann::json;

class Config {
public:
    Config(const std::string& date);
private:
    const std::string config_name_ = "wayhome-config.json";
    std::string api_key_;
    std::string from_;
    std::string to_;
    std::string date_;
    std::time_t curr_time_ = std::time_t(nullptr);

    void CreateConfigFile();
    bool CheckJson(const json& data);
    bool IsValidDate(const std::string& date);
};
