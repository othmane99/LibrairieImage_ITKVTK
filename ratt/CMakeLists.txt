CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

PROJECT(ratt)

find_package( ITK REQUIRED )
include( ${ITK_USE_FILE} )

find_package(VTK REQUIRED)

if (NOT VTK_FOUND)
  message("Skipping ratt: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")

if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(ratt MACOSX_BUNDLE Circle.cpp 
  itkImageToVTKImageFilter.h  itkImageToVTKImageFilter.hxx)
  target_link_libraries(ratt PRIVATE ${VTK_LIBRARIES} ${ITK_LIBRARIES})
else ()
  # include all components
  add_executable(ratt MACOSX_BUNDLE Circle.cpp itkImageToVTKImageFilter.h  itkImageToVTKImageFilter.hxx)
  target_link_libraries(ratt PRIVATE ${VTK_LIBRARIES} ${ITK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS ratt
    MODULES ${VTK_LIBRARIES}
    )
endif ()
