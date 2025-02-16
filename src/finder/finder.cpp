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

std::string Cache::GetYandexCode(const std::string& city) {
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

    std::cout << " <BOOM. PERVII. > " << from_code_ << " " << to_code_ << '\n';
    return true;
}