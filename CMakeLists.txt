cmake_minimum_required(VERSION 3.22)
project(randomizer VERSION 0.1.0)

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

# Make CPack install the binary in /usr/bin
install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)

# Make CPack install the man page
include(GNUInstallDirs)
install(FILES "${CMAKE_SOURCE_DIR}/docs/randomizer.1" DESTINATION "${CMAKE_INSTALL_MANDIR}/man1")

# Add compiler warnings only for GCC and Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(randomizer_lib PRIVATE
        -Wall
        -Wextra
        -Wshadow
        -Wconversion
        -Wpedantic
        -Werror
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wshadow
        -Wconversion
        -Wpedantic
        -Werror
    )
endif()


enable_testing()

# Do not want to include Google Test in the installation packages
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_subdirectory(packaging)