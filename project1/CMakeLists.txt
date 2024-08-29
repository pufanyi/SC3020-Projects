cmake_minimum_required(VERSION 3.12)
project(project1 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add your source files here
aux_source_directory(src SOURCES)

# Add your header files here
include_directories(include)

# Add your executable target
add_executable(out ${SOURCES})
