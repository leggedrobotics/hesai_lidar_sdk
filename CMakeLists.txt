cmake_minimum_required(VERSION 3.8.11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
PROJECT(Template)
set(VERSION_MAJOR 2)
set(VERSION_MINOR 0)
set(VERSION_TINY  5)
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/Version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/Version.h"
)

cmake_policy(SET CMP0053 NEW)
set(CMAKE_BUILD_TYPE ON)
set(CMAKE_BUILD_TYPE Release)
add_definitions(-O3)
add_compile_options(-Wall)

# 设置生成平台为 x64
if(_MSC_VER)
set(CMAKE_GENERATOR_PLATFORM x64)
set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
endif()


include_directories("${PROJECT_BINARY_DIR}")

set(CATKIN_PACKAGE_DEPENDENCIES
  roscpp
  roslib
  rosbag
  )

find_package(roscpp REQUIRED)
find_package(rosbag REQUIRED)
find_package(roslib REQUIRED)

find_package(Boost REQUIRED COMPONENTS system filesystem thread)
find_package(OpenSSL REQUIRED)
add_definitions(-DQT_MESSAGELOGCONTEXT)
add_subdirectory(libhesai)

# set (Boost_INCLUDE_DIRS "/usr/lib/x86_64-linux-gnu/")

include_directories(
  libhesai
  libhesai/Lidar
  libhesai/UdpParser
  libhesai/UdpParser/include
  libhesai/UdpParser/src
  libhesai/UdpProtocol
  libhesai/Source/include
  libhesai/Container/include
  libhesai/Container/src
  libhesai/UdpParserGpu
  libhesai/UdpParserGpu/include
  libhesai/UdpParserGpu/src
  libhesai/PtcClient/include
  libhesai/PtcParser
  libhesai/PtcParser/src
  libhesai/PtcParser/include
  libhesai/Logger/include
  libhesai/include
  driver
  third_party
  ${Boost_INCLUDE_DIRS}
  ${OPENSSL_INCLUDE_DIR}
)
link_directories(./libhesai/PtcClient/lib)
link_directories(${Boost_LIBRARY_DIRS})

add_executable(sample
  test/test.cc
)

target_link_libraries(sample
  ${Boost_LIBRARIES}
  source_lib
  container_lib
  ptcClient_lib
  ptcParser_lib
  log_lib
  # ${PCL_LIBRARIES}  
  ${OPENSSL_LIBRARIES}
)

# add_library(libhesai
#     driver/hesai_lidar_sdk.hpp
# )
# target_link_libraries(libhesai
#   ${Boost_LIBRARIES}
#   source_lib
#   container_lib
#   ptcClient_lib
#   log_lib
  
# )


