cmake_minimum_required(VERSION 3.10.2)

project( rgb )

find_package( ITK REQUIRED )
include( ${ITK_USE_FILE} )

find_package(VTK REQUIRED)

if (NOT VTK_FOUND)
  message("Skipping rgb: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")

if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(rgb MACOSX_BUNDLE rgb.cpp actor.h
  itkImageToVTKImageFilter.h  itkImageToVTKImageFilter.hxx)
  target_link_libraries(rgb PRIVATE ${VTK_LIBRARIES} ${ITK_LIBRARIES})
else ()
  # include all components
  add_executable(rgb MACOSX_BUNDLE rgb.cpp actor.h itkImageToVTKImageFilter.h  itkImageToVTKImageFilter.hxx)
  target_link_libraries(rgb PRIVATE ${VTK_LIBRARIES} ${ITK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS rgb
    MODULES ${VTK_LIBRARIES}
    )
endif ()
