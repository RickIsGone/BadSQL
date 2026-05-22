cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
project(BadSQL VERSION 0.1.0 LANGUAGES CXX)

file(GLOB SOURCES "src/*.cpp")
add_executable(BadSQL ${SOURCES})