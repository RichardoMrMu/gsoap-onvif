# WorkLoop

find_package(OpenCV REQUIRED core imgproc highgui)

# turn on C++11
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/3rdparty/FindGlog.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/3rdparty/ssigonvif/ssig-onvif.cmake)

# workloop
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
file(GLOB WORKLOOP_INC ${CMAKE_CURRENT_LIST_DIR}/include/*.h*)
file(GLOB WORKLOOP_SRC ${CMAKE_CURRENT_LIST_DIR}/src/*.h*
                       ${CMAKE_CURRENT_LIST_DIR}/src/*.c*)
file(GLOB WORKLOOP_MOD_INC ${CMAKE_CURRENT_LIST_DIR}/include/module/*.h*)
file(GLOB WORKLOOP_MOD_SRC ${CMAKE_CURRENT_LIST_DIR}/src/module/*.h*
                           ${CMAKE_CURRENT_LIST_DIR}/src/module/*.c*)
source_group(include FILES ${WORKLOOP_INC})
source_group(include\\module FILES ${WORKLOOP_MOD_INC})
source_group(src FILES ${WORKLOOP_SRC})
source_group(src\\module FILES ${WORKLOOP_MOD_SRC})
source_group(src\\module\\internal FILES ${WORKLOOP_MOD_INTERNAL_SRC})

#add pthreads
set(PTHREAD_LIBS -lpthread)

set(WORKLOOP_EXTRA_LIBS ${OpenCV_LIBS} ssigonvif onvif glog ${PTHREAD_LIBS})


set(WORKLOOP_ALL ${WORKLOOP_INC} ${WORKLOOP_SRC}
                 ${WORKLOOP_MOD_INC} ${WORKLOOP_MOD_SRC}
                 ${WORKLOOP_MOD_INTERNAL_SRC})
add_library(wl STATIC ${WORKLOOP_ALL})
target_link_libraries(wl ${GLOG_LIBRARY} ${WORKLOOP_EXTRA_LIBS})
