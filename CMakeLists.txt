cmake_minimum_required(VERSION 2.8)
project(ros_node_control_lib)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost 1.40 COMPONENTS date_time thread REQUIRED)

include_directories(
    include
    ${Boost_INCLUDE_DIR}
)

set(ROS_NODE_CONTROL_LIB_HEADERS include/ros_node_info.hpp include/ros_node_control.hpp)
set(ROS_NODE_CONTROL_LIB_SOURCES src/ros_node_info.cpp src/ros_node_control.cpp)

add_library(ros_node_control_lib SHARED ${ROS_NODE_CONTROL_LIB_HEADERS} ${ROS_NODE_CONTROL_LIB_SOURCES})
target_link_libraries(ros_node_control_lib pthread)

find_library(KEYBOARD_LIB_FILE keyboard_lib)

add_executable(ros_node_control_lib_test examples/ros_node_control_lib_test.cpp)
target_link_libraries(ros_node_control_lib_test ros_node_control_lib pthread ${KEYBOARD_LIB_FILE} ${Boost_LIBRARIES})

install(TARGETS ros_node_control_lib DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES ${ROS_NODE_CONTROL_LIB_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(TARGETS ros_node_control_lib_test DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
