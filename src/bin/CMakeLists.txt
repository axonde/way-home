add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(${PROJECT_NAME} PRIVATE config)
target_link_libraries(${PROJECT_NAME} PRIVATE finder)
target_link_libraries(${PROJECT_NAME} PRIVATE utils)