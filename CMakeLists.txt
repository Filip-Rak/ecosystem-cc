cmake_minimum_required(VERSION 3.16)
project(Ecosystem)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/dependencies.cmake)
add_subdirectory(src)