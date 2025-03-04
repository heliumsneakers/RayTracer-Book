cmake_minimum_required(VERSION 3.15)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json (optional, useful for IDEs)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# If using GCC or Clang, you can globally add ASan flags.
if (CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif ()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCE_FILES
    src/*.cpp
    src/*.c
)

# ----------------------------
# Create the Executable
# ----------------------------
add_executable(RayTracer ${SOURCE_FILES})

