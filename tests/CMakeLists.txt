# Download and unpack GoogleTest if not present
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Collect test sources
file(GLOB_RECURSE TEST_SOURCES "*.cpp")

# Add test executable
add_executable(RandomizerTests ${TEST_SOURCES})

# Link to GoogleTest and your app sources
target_include_directories(RandomizerTests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(RandomizerTests gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(RandomizerTests)
