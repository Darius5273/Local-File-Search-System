cmake_minimum_required(VERSION 3.27)
project(Local_File_Search_Backend)

set(CMAKE_CXX_STANDARD 17)

add_executable(Local_File_Search_Backend src/main.cpp
        include/FileMetadata.h
        include/SearchResult.h
        src/FileMetadata.cpp
        src/SearchResult.cpp
        include/DatabaseConnector.h
        src/DatabaseConnector.cpp)

set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake")
find_package(libpqxx CONFIG REQUIRED)

target_link_libraries(Local_File_Search_Backend PRIVATE libpqxx::pqxx)