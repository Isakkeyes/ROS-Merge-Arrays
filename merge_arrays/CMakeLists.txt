cmake_minimum_required(VERSION 3.8)
project(merge_arrays)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(merge_arrays_node src/merge_arrays_node.cpp)
ament_target_dependencies(merge_arrays_node rclcpp std_msgs)

install(TARGETS
  merge_arrays_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()
