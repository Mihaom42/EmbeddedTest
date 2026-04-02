cmake_minimum_required(VERSION 3.8)
project(gps_package)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

include_directories(include)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/GpsMsg.msg"
)

add_executable(gps_node
  src/gps_node.cpp
  src/PacketHandler.cpp
)

ament_target_dependencies(gps_node
  rclcpp
  std_msgs
)

rosidl_target_interfaces(gps_node
  ${PROJECT_NAME} "rosidl_typesupport_cpp"
)

install(TARGETS
  gps_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_export_dependencies(rosidl_default_runtime)

ament_package()