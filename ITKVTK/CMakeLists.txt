cmake_minimum_required(VERSION 3.10.2)

project( Detection )

find_package( ITK REQUIRED )
include( ${ITK_USE_FILE} )

find_package(VTK REQUIRED)

if (NOT VTK_FOUND)
  message("Skipping Detection: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")

if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(Detection MACOSX_BUNDLE Detection.cpp 
       itkImageToVTKImageFilter.hxx itkImageToVTKImageFilter.h )
  target_link_libraries(Detection PRIVATE ${VTK_LIBRARIES} ${ITK_LIBRARIES})
else ()
  # include all components
  add_executable(Detection MACOSX_BUNDLE Detection.cpp 
        itkImageToVTKImageFilter.hxx itkImageToVTKImageFilter.h)
  target_link_libraries(Detection PRIVATE ${VTK_LIBRARIES} ${ITK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS Detection
    MODULES ${VTK_LIBRARIES}
    )
endif ()
