cmake_minimum_required(VERSION 2.8)

project(Signature C CXX)
set(CMAKE_CXX_STANDARD 17)
find_package (Threads)
include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES
    main.cpp
    FileReader.cpp
    FileReader.h
    FileWriter.cpp
    FileWriter.h
    Semaphore.cpp
    Semaphore.h
    SignatureProcessor.cpp
    SignatureProcessor.h
    ThreadPool.cpp
    ThreadPool.h
    )

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
