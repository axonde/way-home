cmake_minimum_required(VERSION 3.12)

project(
    wayhome
    DESCRIPTION "6th laboratory 1 course SE ITMO"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# Cpp requests
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git)
FetchContent_MakeAvailable(cpr)

# JSON
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_subdirectory(src)
