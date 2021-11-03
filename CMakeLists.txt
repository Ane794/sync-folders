cmake_minimum_required(VERSION 3.20)
project(sync-folders)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)
link_directories(lib)

aux_source_directory(src SYNC_FOLDERS_SOURCES)
add_executable(${PROJECT_NAME} ${SYNC_FOLDERS_SOURCES})

target_link_libraries(${PROJECT_NAME} yaml-cpp)
