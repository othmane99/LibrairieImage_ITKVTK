CMAKE_MINIMUM_REQUIRED(VERSION 3.10) 

PROJECT(Ball) 

# Path for CMake to find dumpbin.exe
LIST(APPEND CMAKE_PROGRAM_PATH  "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/bin/Hostx64/x64")

find_package(VTK REQUIRED)

if (NOT VTK_FOUND)
  message("Skipping Ball: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")
if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(Ball MACOSX_BUNDLE Ball.cpp Ball.h )
  target_link_libraries(Ball PRIVATE ${VTK_LIBRARIES})
else ()
  # include all components
  add_executable(Ball MACOSX_BUNDLE Ball.cpp Ball.h )
  target_link_libraries(Ball PRIVATE ${VTK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS Ball
    MODULES ${VTK_LIBRARIES}
    )
endif ()
