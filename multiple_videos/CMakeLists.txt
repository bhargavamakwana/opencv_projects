cmake_minimum_required (VERSION 3.5.0)
project(Multiple_videos)
# to use C++11
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
      set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    endif()




add_executable(Multiple_videos main.cpp)

#ie_add_sample(NAME security_barrier_camera_demo
#                   SOURCES ${SOURCES}
#                   HEADERS ${HEADERS}
#                   OPENCV_DEPENDENCIES core highgui videoio)



# Find OpenCV components if exist
#if(OPENCV_DEPENDENCIES)
 find_package(OpenCV COMPONENTS core highgui videoio QUIET)
    if(NOT OpenCV_FOUND)
        message(WARNING "OPENCV is disabled or not found,  skipped")
        return()
    else()
        add_definitions(-DUSE_OPENCV)
    endif()
    #endif()

target_link_libraries(Multiple_videos PRIVATE ${OpenCV_LIBRARIES})
if(UNIX)
  target_link_libraries(Multiple_videos PRIVATE pthread)
endif()
