cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)
project(BadSQL VERSION 0.1.0 LANGUAGES CXX)

find_package(Boost REQUIRED)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(_WIN32_WINNT=0x0601)
endif()

file(GLOB SOURCES "src/*.cpp")
add_executable(BadSQL ${SOURCES})

target_link_libraries(BadSQL PRIVATE Boost::boost)
