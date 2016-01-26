# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project (HOME_IO)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

file(GLOB_RECURSE SRCS
  ${PROJECT_SOURCE_DIR}/src/backend/*.cpp
  )
file(GLOB_RECURSE HDRS
  ${PROJECT_SOURCE_DIR}/src/backend/*.hpp
  )
ADD_EXECUTABLE(homeio ${PROJECT_SOURCE_DIR}/src/mains/main_poznan.cpp ${SRCS} ${HDRS})

TARGET_LINK_LIBRARIES(
homeio
pthread
curlpp
curl
ncurses
menu
)
