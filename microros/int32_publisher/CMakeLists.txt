cmake_minimum_required(VERSION 3.5.0)
project(int32_publisher VERSION 0.1.0 LANGUAGES C CXX)

# Specify the include directory for the headers
include_directories(libmicroros/include)

# Specify the directory where the library is located

link_directories(libmicroros)

# Add the executable target
add_executable(int32_publisher main.cpp)

# Link the static library without the 'lib' prefix and '.a' suffix
target_link_libraries(int32_publisher microros)

target_sources(int32_publisher PRIVATE libmicroros/include/custom_transport/custom_transport.cpp)