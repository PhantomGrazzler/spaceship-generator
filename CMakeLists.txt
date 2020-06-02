cmake_minimum_required(VERSION 3.15)

project(spaceship-generator
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

add_executable(spaceship-generator
    src/main.cpp
)

add_custom_command(TARGET spaceship-generator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/data/vehicle_parts.txt ${CMAKE_BINARY_DIR}/
)
