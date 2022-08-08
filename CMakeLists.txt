cmake_minimum_required(VERSION 3.20)
project(Logger CXX)
set(CMAKE_CXX_STANDARD 20)

message(${PROJECT_NAME})

add_subdirectory(spdlog-1.x)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog CONFIG REQUIRED)
endif()

add_library(${PROJECT_NAME} include/Logger.h include/AsyncLogger.h include/SerialLogger.h)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)

target_include_directories(${PROJECT_NAME}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
        )