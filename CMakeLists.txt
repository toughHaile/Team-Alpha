cmake_minimum_required(VERSION 3.15)
project(minigit VERSION 1.0 LANGUAGES CXX)

# Configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type settings
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Werror")

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Source files (manual listing for your flat structure)
set(SOURCES
    main.cpp
    # Add other .cpp files if they exist in root
)

# Include directory
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Executable target
add_executable(minigit ${SOURCES})

# Link libraries
target_link_libraries(minigit PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Installation
install(TARGETS minigit DESTINATION bin)

# Testing (point to your test.sh in root)
enable_testing()
add_test(NAME minigit_test 
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
