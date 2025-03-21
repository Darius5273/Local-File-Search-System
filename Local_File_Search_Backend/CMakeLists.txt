cmake_minimum_required(VERSION 3.27)
project(Local_File_Search)

set(CMAKE_CXX_STANDARD 17)

add_executable(Local_File_Search src/main.cpp
        include/FileMetadata.h
        include/SearchResult.h
        src/FileMetadata.cpp
        src/SearchResult.cpp)
