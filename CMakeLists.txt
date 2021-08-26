project(zoom_tea)
cmake_minimum_required(VERSION 2.8)

list(APPEND CUDA_NVCC_FLAGS "-std=c++11")
SET(CUDA_PROPAGATE_HOST_FLAGS OFF)
set(CMAKE_CXX_FLAGS "-std=c++0x")
find_package(OpenCV REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/factory/workloop.cmake)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
file(GLOB INC include/*.h include/*.hpp)
file(GLOB SRC src/*.c src/*.cc src/*.cpp)

source_group(include FILES ${INC})
source_group(src FILES ${SRC})

add_executable(zoom_tea ${INC} ${SRC})

target_link_libraries(zoom_tea wl ${OpenCV_LIBS})

list(APPEND CUDA_NVCC_FLAGS "-std=c++11")
SET(CUDA_PROPAGATE_HOST_FLAGS OFF)
set(CMAKE_CXX_FLAGS "-std=c++0x")
