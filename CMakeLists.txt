# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Set project name
project(Prunet)

# Set C++ Standard to C++17 (or higher if needed)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the directory for the output binaries
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

# Define the path for the source files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add subdirectories for libraries and components
add_subdirectory(src)

# External dependencies (if any)
# find_package(library_name REQUIRED)  # For example, libp2p or any other external library

# Set build type (Debug, Release, etc.)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Specify the binary output location
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Specify the location of headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add any external dependencies you need here
# For example, for the libp2p dependency, add:
# find_package(libp2p REQUIRED)

# Add any other needed packages here
# find_package(Boost REQUIRED)

# Specify the directories containing source files
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")

# Create executable target
add_executable(prunet ${SOURCE_FILES})

# Link libraries (if needed, for example libp2p or others)
# target_link_libraries(prunet libp2p)

# Add flags for debugging and optimization
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra")

# Add installation rule if you want to install the binary
# install(TARGETS prunet DESTINATION /usr/local/bin)
