cmake_minimum_required(VERSION 3.22)


# Download and unpack GoogleTest if not present
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Prevent GoogleTest from being installed by CPack
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(INSTALL_GMOCK OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Collect test sources
file(GLOB_RECURSE TEST_SOURCES "*.cpp")

# Add test executable
add_executable(RandomizerTests ${TEST_SOURCES})

# Link to GoogleTest and the library for the randomizer code
target_link_libraries(RandomizerTests PRIVATE gtest gtest_main randomizer_lib)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(RandomizerTests PRIVATE
        -Wall
        -Wextra
        -Wshadow
        -Wconversion
        -Wpedantic
        -Werror
    )
endif()

include(GoogleTest)
gtest_discover_tests(RandomizerTests)
