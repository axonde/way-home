#pragma once
#include <iterator>
#include <list>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <sstream>
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
    std::string GetYandexCode(const std::string& city) const;
    const std::string& Request(const std::string& from_code, const std::string& to_code);

    const Config* const config_;
private:
    const std::string data_filename_ = "wayhome-data.json";

    /// @brief cache/wayhome-req-<from>-<to>-YYYY-MM-DD.json
    /// @attention the from and to is yandex code.
    std::filesystem::path cached_request_filename_;

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
    const std::list<std::string>& Find();
private:
    Cache cache_;
    std::string from_code_;
    std::string to_code_;
    std::list<std::string> itineraries;
};
