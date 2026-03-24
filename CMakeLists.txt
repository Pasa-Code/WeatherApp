cmake_minimum_required(VERSION 3.20)

project(PG2_Projekte_GitHub)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 1.10.5)
FetchContent_MakeAvailable(cpr)

add_executable(PG2_Projekte_GitHub main.cpp api_key.h)

target_link_libraries(PG2_Projekte_GitHub PRIVATE cpr::cpr nlohmann_json::nlohmann_json)