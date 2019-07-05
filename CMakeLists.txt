cmake_minimum_required(VERSION 2.8)

project(Signature C CXX)
set(CMAKE_CXX_STANDARD 11)
find_package (Threads)
include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES
    main.cpp
    FileReader.h
    FileReader.cpp
    Semaphore.cpp
    Semaphore.h
    SignatureProcessor.cpp
    SignatureProcessor.h
    )

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
