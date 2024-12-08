cmake_minimum_required(VERSION 3.12)
project(project1 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-g)

# Add your source files here
aux_source_directory(src SOURCES)

# Add your header files here
include_directories(include)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
aux_source_directory(test TEST_SOURCES)

add_executable(
    unittest
    ${TEST_SOURCES}
    ${PROJECT_SOURCE_DIR}/src/block.cpp
    ${PROJECT_SOURCE_DIR}/src/data_loader.cpp
    ${PROJECT_SOURCE_DIR}/src/block_ptr.cpp
    ${PROJECT_SOURCE_DIR}/src/utils.cpp
    ${PROJECT_SOURCE_DIR}/src/file_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/database_manager.cpp
    ${PROJECT_SOURCE_DIR}/src/fields.cpp
    ${PROJECT_SOURCE_DIR}/src/schema.cpp
    ${PROJECT_SOURCE_DIR}/src/buffer.cpp
    ${PROJECT_SOURCE_DIR}/src/data_ptr.cpp
    ${PROJECT_SOURCE_DIR}/src/record.cpp
    ${PROJECT_SOURCE_DIR}/src/block_list.cpp
    ${PROJECT_SOURCE_DIR}/src/b_plus_tree.cpp
    ${PROJECT_SOURCE_DIR}/src/index.cpp
    # Note: it is possible to use file(GLOB ...) to add all files in a directory
    # but it is not recommended by CMake and the suggested way is to list all files
    # Add your source files here
    )
target_link_libraries(unittest gtest_main)
add_test(NAME UnitTest COMMAND unittest)

# Add your executable target
add_executable(project1 ${SOURCES})
