cmake_minimum_required(VERSION 3.20)
project(csv_huawei_lte_log_lib VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)

add_library(csv_huawei_lte_log_lib
    src/logger.cpp
    src/catalog_path_resolver.cpp
    src/file_processor.cpp
)

target_include_directories(csv_huawei_lte_log_lib PUBLIC include)
target_link_libraries(csv_huawei_lte_log_lib PUBLIC spdlog::spdlog yaml-cpp)

add_executable(example_run src/main.cpp)
target_link_libraries(example_run PRIVATE csv_huawei_lte_log_lib)
