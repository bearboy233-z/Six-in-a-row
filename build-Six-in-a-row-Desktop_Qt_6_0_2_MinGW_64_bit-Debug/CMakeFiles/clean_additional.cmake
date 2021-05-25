# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Six-in-a-row_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Six-in-a-row_autogen.dir\\ParseCache.txt"
  "Six-in-a-row_autogen"
  )
endif()
