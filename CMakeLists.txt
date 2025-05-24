cmake_minimum_required(VERSION 3.22)
project(randomizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(include)

# Automatically collect all source and header files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

# Define a library target for the main code
add_library(randomizer_lib ${SOURCES} ${HEADERS})
target_include_directories(randomizer_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)


# Define main executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE randomizer_lib)


enable_testing()
add_subdirectory(tests)