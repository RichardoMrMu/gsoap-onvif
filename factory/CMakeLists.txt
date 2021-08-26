project(test CXX)
cmake_minimum_required(VERSION 2.8)

list(APPEND CUDA_NVCC_FLAGS "-std=c++11")
SET(CUDA_PROPAGATE_HOST_FLAGS OFF)
set(CMAKE_CXX_FLAGS "-std=c++0x")

include(${CMAKE_CURRENT_LIST_DIR}/workloop.cmake)

include_directories(${CMAKE_CURRENT_LIST_DIR}/test)
file(GLOB SRC test/*.c*)

source_group(src FILES ${SRC})
add_executable(test ${SRC})
find_package(Curses REQUIRED)
target_link_libraries(test wl ${CURSES_LIBRARIES} -lpthread)
target_include_directories(test PUBLIC ${CURSES_INCLUDE_DIR})

list(APPEND CUDA_NVCC_FLAGS "-std=c++11")
SET(CUDA_PROPAGATE_HOST_FLAGS OFF)
set(CMAKE_CXX_FLAGS "-std=c++0x")
