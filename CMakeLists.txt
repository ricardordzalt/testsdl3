cmake_minimum_required(VERSION 3.15)
project(SDL3_Vulkan_Renderer)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL3 REQUIRED CONFIG)
find_package(Vulkan REQUIRED)

add_executable(main src/main.cpp)

target_link_libraries(main SDL3::SDL3 Vulkan::Vulkan)
