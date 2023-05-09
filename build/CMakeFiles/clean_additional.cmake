# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/FirstPersonCamera_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/FirstPersonCamera_autogen.dir/ParseCache.txt"
  "FirstPersonCamera_autogen"
  )
endif()
