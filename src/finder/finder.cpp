#include "finder.h"

Cache::Cache() {
    // not good idea to implement this here, will think about it seriously a little bit later.

    // try to cache our request beforehand
    std::filesystem::path req_path(cached_request_filename_ + "");
    // if (std::filesystem::exists)
}

void Cache::InitializeData() {
    std::fstream data(data_filename_, data.in);
    if (!data.is_open()) {
        cpr::Response data_req_ = cpr::Get(cpr::Url(stations_list_url_));
        if (data_req_.status_code != 200) {
            throw ErrorRequestBadStatus();
        }
        std::fstream data(data_filename_, data.out);
        data << data_req_.text;
    }
}

Finder::Finder() {
    
}