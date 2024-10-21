# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Projectcpp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Projectcpp_autogen.dir\\ParseCache.txt"
  "Projectcpp_autogen"
  )
endif()
