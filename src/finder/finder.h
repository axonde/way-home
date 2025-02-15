#pragma once
#include <fstream>
#include <string>
#include <cpr/cpr.h>

class Cache {
public:
    Cache() {}
    void InitializeData();
private:
    const std::string data_filename_ = "wayhome-data.json";

    /// @brief cache/wayhome-req-YYYY-MM-DD-<from>-<to>.json
    /// @attention the from and to is yandex code.
    const std::string cached_request_filename_ = "cache/wayhome-req-";

    const std::string stations_list_url_ = "https://api.rasp.yandex.net/v3.0/stations_list";
    const std::string between_stations_url_ = "https://api.rasp.yandex.net/v3.0/search";
    std::string data_;
    std::string request_;
};

class Finder {
public:
    Finder();
private:
    Cache cache_;
};
