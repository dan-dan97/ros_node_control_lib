# ros_node_control_lib
A library to control ros nodes and launch files

# Linux Installation

1. Navigate to *ros_node_control_lib* root directory
  * `cd .../ros_node_control_lib`

2. Compile
  Create build folder
  * `mkdir build && cd build`
  Run cmake
  * `cmake ../`
  Recompile *ros_node_control_lib*
  * `make clean && make && sudo make install`
  Now the library is installed to /usr/local/include and /usr/local/lib. Example is installed to /usr/local/bin.
