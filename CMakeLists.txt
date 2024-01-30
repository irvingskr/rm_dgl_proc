cmake_minimum_required(VERSION 3.27)
project(rm_dgl_proc)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

include_directories(include)

add_executable(${PROJECT_NAME}
        src/rm_dgl_proc.cpp)
add_executable(test_n
        src/main.cpp)

target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS})
target_link_libraries(test_n
        ${OpenCV_LIBS})
