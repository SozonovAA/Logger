cmake_minimum_required(VERSION 3.20)

project(boost_log CXX )
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Logger)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} Logger)