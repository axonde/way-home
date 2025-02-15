add_library(config config.cpp)

target_link_libraries(config PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(config PRIVATE utils)
