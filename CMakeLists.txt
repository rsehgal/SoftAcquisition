cmake_minimum_required(VERSION 3.12)
project(SoftSiPM_UT VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard (change to 20 if your project requires C++20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(SoftSiPM)
add_subdirectory(SoftReadout)
