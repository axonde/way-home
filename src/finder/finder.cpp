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
    try { request_raw = cache_.Request(from_code_, to_code_); }
    catch (...) { throw; }

    json request = json::parse(request_raw);
    
    if (request["pagination"]["total"] == 0) return itineraries;

    for (const auto& segment : request["segments"]) {
        std::stringstream way;
        bool has_transfers = true;
        if (const auto iter = segment.find("has_transfers"); iter == segment.end()) {
            Warnings::ErrorIllJsonAnswer();
            continue;
        } else { has_transfers = *iter; }

        auto get = [&](json::const_iterator iter, std::string param = "") -> std::string {
            if (iter == segment.end()) { return "_x_"; }
            if (param.empty()) {
                if (iter->is_string()) { return iter->get<std::string>(); }
                else if (iter->is_number()) { return std::to_string(iter->get<int>()); }
                else { return "?"; }
            } else {
                if (auto inner_iter = iter->find(param); inner_iter != iter->end()) {
                    if (inner_iter->is_string()) { return inner_iter->get<std::string>(); }
                    else if (inner_iter->is_number()) { return std::to_string(inner_iter->get<int>()); }
                    else { return "?"; }
                } else { return "_x_"; }
            }
        };

        if (!has_transfers) {
            const std::map<std::string, json::const_iterator> params {
                {"from", segment.find("from")},
                {"to", segment.find("to")},
                {"departure", segment.find("departure")},
                {"arrival", segment.find("arrival")},
                {"thread", segment.find("thread")},
                {"duration", segment.find("duration")}
            };
            way << "Route " << get(params.at("from"), "title") << " (" << get(params.at("from"), "station_type_name") << ") >> "
                            << get(params.at("to"), "title") << " (" << get(params.at("to"), "station_type_name") << ") "
                            << "[without transfers]" << '\n';
            way << "\t- " << get(params.at("departure")) << " -> " << get(params.at("arrival")) << '\n';
            way << "\t  " << get(params.at("thread"), "title") << " :: by " << get(params.at("thread"), "transport_type") << "  ~" << get(params.at("duration")) << " сек.";
        }
        else {
            const std::map<std::string, json::const_iterator> params {
                {"departure_from", segment.find("departure_from")},  // text
                {"arrival_to", segment.find("arrival_to")},  // text
                {"departure", segment.find("departure")},  // time
                {"arrival", segment.find("arrival")},  // time
                {"details", segment.find("details")},
                {"duration", segment.find("duration")},
                {"transfers", segment.find("transfers")}
            };
            way << "Route " << get(params.at("departure_from"), "title") << " (" << get(params.at("departure_from"), "station_type_name") << ") >> "
                            << get(params.at("arrival_to"), "title") << " (" << get(params.at("arrival_to"), "station_type_name") << ") "
                            << "[with transfers (" << (params.at("transfers") != segment.end() ? std::to_string(params.at("transfers")->size()) : "?") << ")]" << '\n';
            if (params.at("details") == segment.end()) {
                way << "\t- Bad API results.";
                continue;
            }
            for (const auto& part : *params.at("details")) {
                if (const auto is_transfer = part.find("is_transfer"); is_transfer != part.end() && *is_transfer) {  // пересадка
                    const auto transfer_from = part.find("transfer_fom");
                    const auto transfer_to = part.find("transfer_to");
                    if (transfer_from == part.end() || transfer_to == part.end()) {
                        way << "\t- Bad API results.";
                        continue;
                    }
                    way << "\t- " << get(transfer_from, "title") << " -> " << get(transfer_to, "title") << "  ~" << get(part.find("duration")) << " сек." << '\n';
                } else {  // действ. движение
                    const auto thread = part.find("thread");
                    if (thread == part.end()) {
                        way << "\t- Bad API results.";
                        continue;
                    }
                    // way << "\t- " << get(part.find("departure")) << " -> " << get(part.find("arrival")) << '\n';
                    // way << "\t  " << get(thread, "title") << " :: by " << get(thread, "transport_type") << "  ~" << get(part.find("duration")) << " сек.";
                }
            }
        }

        itineraries.push_back(way.str());
    }


    return itineraries;
}
