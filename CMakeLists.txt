cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(randomizer VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(include)

# Automatically collect all source and header files
set(SOURCES
  "src/runner.cpp"
  "src/prngs/linear_congruential_generator.cpp"
  "src/prngs/mersenne_twister.cpp"
  "src/prngs/prng.cpp"
  "src/prngs/xorshift.cpp"
)
set(HEADERS
  "include/linear_congruential_generator.hpp"
  "include/mersenne_twister.hpp"
  "include/prng.hpp"
  "include/program_runner.hpp"
  "include/xorshift.hpp"
)

# Define a library target for the main code
add_library(randomizer_lib ${SOURCES} ${HEADERS})
target_include_directories(randomizer_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)


# Define main executable
add_executable(${PROJECT_NAME} "src/main.cpp")
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

add_subdirectory(tests)

add_subdirectory(packaging)
