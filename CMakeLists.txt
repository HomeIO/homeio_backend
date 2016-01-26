# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required(VERSION 3.0.0)
project (HOME_IO)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
set(CMAKE_BUILD_TYPE Release)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-O2")        ## Optimize
    set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary
endif()

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX14)
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} use C++14")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX11)
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} use C++11")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} use old C++14 - C++0x")
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
ADD_EXECUTABLE(homeio_$ENV{HOMEIO_SITE} ${PROJECT_SOURCE_DIR}/src/mains/main_$ENV{HOMEIO_SITE}.cpp ${SRCS} ${HDRS})

TARGET_LINK_LIBRARIES(
homeio_$ENV{HOMEIO_SITE}
pthread
curlpp
curl
ncurses
menu
)
