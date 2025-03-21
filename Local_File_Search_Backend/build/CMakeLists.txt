cmake_minimum_required(VERSION 3.27)
project(Local_File_Search)

set(CMAKE_CXX_STANDARD 17)

add_executable(Local_File_Search ../src/main.cpp
        ../include/FileMetadata.h
        ../include/SearchResult.h
        ../src/FileMetadata.cpp
        ../src/SearchResult.cpp
        ../include/DatabaseConnector.h)

set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake")
find_package(LIBPQXX CONFIG REQUIRED)

target_link_libraries(Local_File_Search PRIVATE pqxx)