#pragma once
#include <iterator>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <utils/utils.h>
#include <config/config.h>

using json = nlohmann::json;

class Cache {
public:
    Cache(Config* config) : config_(config) {}
    void InitializeData();
    std::string GetYandexCode(const std::string& city);

    const Config* const config_;
private:
    const std::string data_filename_ = "wayhome-data.json";

    /// @brief cache/wayhome-req-<from>-<to>-YYYY-MM-DD.json
    /// @attention the from and to is yandex code.
    const std::string cached_request_filename_ = "cache/wayhome-req-";

    const std::string stations_list_url_ = "https://api.rasp.yandex.net/v3.0/stations_list";
    const std::string search_url_ = "https://api.rasp.yandex.net/v3.0/search";
    std::string data_;
    std::string request_;
};

class Finder {
public:
    Finder(Config* config) : cache_(config) {}
    bool InitializeData();
    bool SetYandexCodes();
private:
    Cache cache_;
    std::string from_code_;
    std::string to_code_;
};
