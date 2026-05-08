cmake_minimum_required(VERSION 3.16)
project(Together)

set(CMAKE_CXX_STANDARD 23)

find_package(PkgConfig REQUIRED)

if(PkgConfig_FOUND)
  pkg_check_modules(JACK jack)
endif()

add_executable(Together
    src/main.cpp
)

target_include_directories(Together PUBLIC ${JACK_INCLUDE_DIRS} include)

target_link_libraries(Together ${JACK_LIBRARIES})