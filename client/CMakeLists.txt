cmake_minimum_required(VERSION 3.10)

project(Lab3_Client)

SET(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")

add_executable(fclient
    main.c
    request.c
    client.c
)

target_include_directories(fclient PUBLIC "${PROJECT_BINARY_DIR}")

TARGET_LINK_LIBRARIES(fclient
    pthread
)