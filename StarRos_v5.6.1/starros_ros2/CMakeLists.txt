cmake_minimum_required(VERSION 3.8)
project(starros)

SET(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 14)

add_compile_options(-Wall -Wextra -Wpedantic)

add_definitions(-DPROJECT_PATH="${PROJECT_SOURCE_DIR}")
add_definitions(-DROS2)
# add_definitions(-DPCAP)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(message_filters REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

link_directories(
  3rdparty/sdk/lib
)

include_directories(
  3rdparty/sdk/include

  include/ringbuffer
  include/common
  include/config_manage
  include/input
  include/calc
  include/publish
)

add_executable( driver_node
  src/driver/driver.cpp
  include/ringbuffer/lwrb.c
  include/ringbuffer/lwrb_ex.c)
target_link_libraries(driver_node ${rclcpp_LIBRARIES} ${YAML_CPP_LIBRARIES})
target_link_libraries(driver_node pthread)
# target_link_libraries(driver_node pcap)
target_link_libraries(driver_node StarSDK_4 DevCtrl)
ament_target_dependencies(driver_node rclcpp sensor_msgs)

#### tool ####
add_executable(fusion_node
  src/tools/fusion.cpp
)
target_link_libraries(fusion_node ${rclcpp_LIBRARIES})
ament_target_dependencies(fusion_node rclcpp sensor_msgs message_filters)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

install(
  TARGETS driver_node fusion_node
  RUNTIME DESTINATION lib/${PROJECT_NAME})

install( 
  DIRECTORY 3rdparty/sdk/lib/
  DESTINATION lib/)

install(
  DIRECTORY launch rviz
  DESTINATION share/${PROJECT_NAME})

ament_package()
