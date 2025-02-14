add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(wayhome PRIVATE cpr::cpr)
target_link_libraries(wayhome PRIVATE nlohmann_json::nlohmann_json)
