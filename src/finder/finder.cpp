#include "finder.h"

void Cache::InitializeData() {
    std::fstream data(data_filename_, std::ios::in);
    if (!data.is_open()) {
        std::string apikey = config_->GetParameters()["apikey"];
        cpr::Response data_req_ = cpr::Get(cpr::Url{stations_list_url_},
                                  cpr::Parameters{{"apikey", apikey}, {"lang", "ru_RU"}, {"format", "json"}});
        if (data_req_.status_code != 200) {
            throw Errors::ErrorRequestBadStatus(data_req_.status_code);
        }
        std::fstream data(data_filename_, data.out);
        data << data_req_.text;
    }
    data.close();
    data.open(data_filename_, std::ios::in);
    data_ = {std::istreambuf_iterator<char>(data), std::istreambuf_iterator<char>()};
}

std::string Cache::GetYandexCode(const std::string& city) const {
    std::fstream data(data_filename_, std::ios::in);
    
    if (!data.is_open()) {
        throw Errors::CantOpenFile();
    }

    json data_json;
    try {
        data_json = json::parse(std::string{std::istreambuf_iterator<char>(data), std::istreambuf_iterator<char>()});
    } catch (...) {
        throw Errors::ErrorParseJson();
    }

    for (const auto& country : data_json["countries"]) {
        for (const auto& region : country["regions"]) {
            for (const auto& settlement : region["settlements"]) {
                if (settlement["title"] == city) {
                    if (settlement["codes"].empty() || settlement["codes"].find("yandex_code") == settlement["codes"].end()) {
                        throw Errors::ErrorApiCode();
                    }
                    return settlement["codes"]["yandex_code"];
                }
            }
        }
    }

    throw Errors::ErrorApiCityNotFound(city);
}

const std::string& Cache::Request(const std::string& from_code, const std::string& to_code) {
    cached_request_filename_ = {"wayhome-cache/wayhome-req-" + from_code + "-" + to_code + "-" + config_->GetParameters().at("date") + ".json"};
    
    std::fstream request_cache{cached_request_filename_, std::ios::in};
    if (request_cache.is_open()) {
        request_ = std::string{std::istreambuf_iterator<char>(request_cache), std::istreambuf_iterator<char>()};
        return request_;
    }

    const auto params = config_->GetParameters();
    cpr::Response request = cpr::Get(
        cpr::Url(search_url_),
        cpr::Parameters{
            {"apikey", params.at("apikey")},
            {"from", from_code}, {"to", to_code},
            {"format", "json"},
            {"date", params.at("date")},
            {"transfers", "true"}
        }
    );

    if (request.status_code != 200) {
        throw Errors::ErrorRequestBadStatus(request.status_code);
    }

    request_cache.close();

    if (!std::filesystem::exists(cached_request_filename_.parent_path())) {
        std::filesystem::create_directories(cached_request_filename_.parent_path());
    }

    request_cache.open(cached_request_filename_, std::ios::out);
    request_cache << request.text;

    return request_;
}


bool Finder::InitializeData() {
    try {
        cache_.InitializeData();
    } catch (const Errors::Error& e) {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

bool Finder::SetYandexCodes() {
    std::string from, to;
    const auto params = cache_.config_->GetParameters();
    from = params.at("from");
    to = params.at("to");
    try {
        from_code_ = cache_.GetYandexCode(from);
        to_code_ = cache_.GetYandexCode(to);
    } catch (const Errors::Error& e) {
        std::cerr << e.what() << '\n';
        return false;
    }
    return true;
}

const std::list<std::string>& Finder::Find() {
    std::string_view request_raw;
    try {
        request_raw = cache_.Request(from_code_, to_code_);
    } catch (const Errors::Error& e) {
        std::cerr << e.what();
        return itineraries;
    }

    json request = json::parse(request_raw);
    
    if (request["pagination"]["total"] == 0) return itineraries;

    for (const auto& segment : request["segments"]) {
        std::stringstream way;
        if (!segment["has_transfers"]) {
            way << "Route " << segment["from"]["title"].get<std::string>() << " (" << segment["from"]["station_type_name"].get<std::string>() << ") -- "
                            << segment["to"]["title"].get<std::string>() << " (" << segment["to"]["station_type_name"].get<std::string>() << ") "
                            << "[without transfers]" << '\n';

            // bad implementation -> look to coolect info in map and after insert in way have or not have (always has error!)

            way << "\t- " << segment["departure"] << " >> " << segment["arrival"] << '\n';
            // std::cout << (segment["thread"].find("transport_type") != segment["thread"].end());
            way << "\t  " << segment["thread"]["title"].get<std::string>() << " :: " << segment["thread"]["transport_type"].get<std::string>() << " ~" << segment["duration"] << " секунд";
        }
        else {
            /// ... 
        }

        itineraries.push_back(way.str());
    }


    return itineraries;
}
