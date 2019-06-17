cmake_minimum_required(VERSION 2.8)

project(Signature)
set(CMAKE_CXX_STANDARD 11)
include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES
    main.cpp
    FileReader.h
    FileReader.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})
