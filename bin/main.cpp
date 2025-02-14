#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

int main() {
    cpr::Response r = cpr::Get(cpr::Url{"https://api.agify.io"}, cpr::Parameters{{"name", "John"}});
    std::cout << "Status " << r.status_code << '\n';
    std::cout << "Response:\n" << r.text << '\n';
    json j = json::parse(r.text);
    std::cout << std::setw(2) << j << '\n';
    std::cout << "Looks good.\n";
    return 0;
}