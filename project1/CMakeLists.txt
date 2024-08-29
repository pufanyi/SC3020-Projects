cmake_minimum_required(VERSION 3.12)
project(SC3020_Project1)

# Add your source files here
aux_source_directory(./src SOURCES)

# Add your header files here
include_directories(./include)

# Add your executable target
add_executable(out ${SOURCES})
