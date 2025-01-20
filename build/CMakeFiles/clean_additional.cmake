# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Project-cpp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Project-cpp_autogen.dir\\ParseCache.txt"
  "Project-cpp_autogen"
  )
endif()
