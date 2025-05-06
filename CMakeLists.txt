cmake_minimum_required(VERSION 3.10.0)
cmake_policy(SET CMP0167 NEW)

include(FetchContent)

# Project definition
project(test-seastar-mem-allocator VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD "20" CACHE STRING "C++ standard")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_BUILD_TYPE "Release")
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build Seastar as a shared library")
set(BUILD_TESTING OFF CACHE BOOL "Build tests")
set(Seastar_DPDK OFF CACHE BOOL "Disable DPDK in Seastar")
set(Seastar_BUILD_TESTING OFF CACHE BOOL "Disable Seastar testing")
set(Seastar_BUILD_DEMOS OFF CACHE BOOL "Disable Seastar demos")
set(Seastar_BUILD_APPS OFF CACHE BOOL "Disable Seastar apps")

# Fetch Seastar
FetchContent_Declare(
    seastar
    GIT_REPOSITORY https://github.com/scylladb/seastar.git
    GIT_TAG 3699c0ce7ea98dc170e6d05148d900b5a649d882
)

FetchContent_MakeAvailable(seastar)

# Add Seastar's CMake directory to the module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${Seastar_SOURCE_DIR}/cmake)

# Library
add_library(my-library SHARED library.cpp)

# Link my-library with Seastar and its dependencies
target_link_libraries(my-library PRIVATE
    seastar
    fmt
    gnutls
    boost_thread
    hwloc
    numa
    yaml-cpp
    dl
    rt
    uring
    boost_program_options
    protobuf
#    stdc++
    pthread
)

# Application
add_executable(application main.cpp)
target_link_libraries(application PRIVATE dl pthread m)

