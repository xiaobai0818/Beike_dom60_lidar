cmake_minimum_required(VERSION 3.0.2)
project(starros)

SET(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 14)

add_definitions(-DPROJECT_PATH="${PROJECT_SOURCE_DIR}")
add_definitions(-DROS1)
# add_definitions(-DPCAP)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  sensor_msgs
  message_filters
)

find_package(yaml-cpp REQUIRED)

catkin_package()

include_directories(
${catkin_INCLUDE_DIRS}
  3rdparty/sdk/include

  include/ringbuffer
  include/common
  include/config_manage
  include/input
  include/calc
  include/publish
)

link_directories(
  3rdparty/sdk/lib
)

add_executable(driver_node
  src/driver/driver.cpp
  include/ringbuffer/lwrb.c
  include/ringbuffer/lwrb_ex.c
)
add_dependencies(driver_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(driver_node ${catkin_LIBRARIES} ${YAML_CPP_LIBRARIES})
target_link_libraries(driver_node pthread)
# target_link_libraries(driver_node pcap)
target_link_libraries(driver_node StarSDK_4 DevCtrl)

#### tool ####
add_executable(fusion_node
  src/tools/fusion.cpp
)
add_dependencies(fusion_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(fusion_node ${catkin_LIBRARIES})
