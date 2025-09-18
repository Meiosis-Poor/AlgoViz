# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\simulator_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\simulator_autogen.dir\\ParseCache.txt"
  "simulator_autogen"
  )
endif()
