cmake_minimum_required(VERSION 3.29)

project(k_queue)
enable_language(C)

set(C_STANDARD          C99)
set(PROJ_PATH           ${CMAKE_CURRENT_SOURCE_DIR})
set(EXECUTABLE          ${CMAKE_PROJECT_NAME})

#
# List of source files to compile
#
set(sources_SRCS
    # Put here your source files, one in each line, relative to CMakeLists.txt file location
    ${PROJ_PATH}/src/k_queue.c
    ${PROJ_PATH}/tests/test.c
)

#
# Include directories
#
set(include_path_DIRS
    # Put here your include dirs, one in each line, relative to CMakeLists.txt file location
    ${PROJ_PATH}/src
    ${PROJ_PATH}/tests
)

#
# Symbols definition
#
set(symbols_SYMB
    # Put here your symbols (preprocessor defines), one in each line
    # Encapsulate them with ouble quotes for safety purpose
)

# Executable files
add_executable(${EXECUTABLE} ${sources_SRCS})

# Include paths
target_include_directories(${EXECUTABLE} PRIVATE ${include_path_DIRS})

# Project symbols
target_compile_definitions(${EXECUTABLE} PRIVATE ${symbols_SYMB})

# Conmpiler options
target_compile_options(${EXECUTABLE} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
)
