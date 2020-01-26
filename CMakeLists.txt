cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)   

project(mcast-listener)


# setting executable output folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# setting source files
set(SOURCES 
    main.cpp
)

# seting executable source and name
add_executable(mcast-listener ${SOURCES})

