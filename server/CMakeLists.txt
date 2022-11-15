cmake_minimum_required(VERSION 3.10)

project(Lab3_Server)

SET(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")

add_executable(fserver
    main.c
)

target_include_directories(fserver PUBLIC "${PROJECT_BINARY_DIR}")

TARGET_LINK_LIBRARIES(fserver
    pthread
)