CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

PROJECT(exam)

find_package( ITK REQUIRED )
include( ${ITK_USE_FILE} )

find_package(VTK REQUIRED)

if (NOT VTK_FOUND)
  message("Skipping exam: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif()
message (STATUS "VTK_VERSION: ${VTK_VERSION}")

if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
  add_executable(exam MACOSX_BUNDLE exam.cpp 
  itkImageToVTKImageFilter.h  itkImageToVTKImageFilter.hxx)
  target_link_libraries(exam PRIVATE ${VTK_LIBRARIES} ${ITK_LIBRARIES})
else ()
  # include all components
  add_executable(exam MACOSX_BUNDLE exam.cpp itkImageToVTKImageFilter.h  itkImageToVTKImageFilter.hxx)
  target_link_libraries(exam PRIVATE ${VTK_LIBRARIES} ${ITK_LIBRARIES})
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS exam
    MODULES ${VTK_LIBRARIES}
    )
endif ()
