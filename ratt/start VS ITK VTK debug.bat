@echo off 
@set CL=/D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE
@set LINK=/LARGEADDRESSAWARE

set ITKDir=K:\lib-x64-msvc2022\ITK-5.2.1-build\bin\Debug
set DCMTKDir=K:\lib-x64-msvc2022\dcmtk-build\bin\Debug
set GDCMDir=K:\lib-x64-msvc2022\GDCM-build\bin\Debug
set VTKDir=K:\lib-x64-msvc2022\VTK-9.1.0-build\bin\Debug
set icuuDir=K:\lib-x64-msvc2022\Anaconda3\Library\bin

PATH=%GDCMDir%;%DCMTKDir%;%ITKDir%;%VTKDir%;%icuuDir%;%PATH%

FOR %%f IN (*.sln) DO %%f

