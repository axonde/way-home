add_library(finder finder.cpp)

target_link_libraries(finder PRIVATE cpr::cpr)
target_link_libraries(finder PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(finder PRIVATE utils)
target_link_libraries(finder PRIVATE config)
