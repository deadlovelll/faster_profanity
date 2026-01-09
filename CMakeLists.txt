cmake_minimum_required(VERSION 3.20)

project(faster_profanity LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(faster_profanity_lib
    src/word_finder/word_finder.cpp
    src/profane_detector/profane_detector.cpp
)

target_include_directories(faster_profanity_lib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)


add_executable(faster_profanity
    src/main.cpp
)

target_link_libraries(faster_profanity
    PUBLIC
        nlohmann_json::nlohmann_json
    PRIVATE
        faster_profanity_lib
)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_Declare(
  nlohmann_json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(nlohmann_json)

enable_testing()
add_subdirectory(tests)