@echo off 
@set CL=/D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE
@set LINK=/LARGEADDRESSAWARE

set ITKDir=C:\Users\oboum\libImage\InsightToolkit-5.2.1-build\bin\Debug
set DCMTKDir=K:\lib-x64-msvc2022\dcmtk-build\bin\Debug
set GDCMDir=K:\lib-x64-msvc2022\GDCM-build\bin\Debug
set VTKDir=C:\Users\oboum\OneDrive\Bureau\LibTraitementImage\VTK-9.2.0.rc2\VTK-9.2.0.rc2-build\bin\Debug
set icuuDir=K:\lib-x64-msvc2022\Anaconda3\Library\bin

PATH=%GDCMDir%;%DCMTKDir%;%ITKDir%;%VTKDir%;%icuuDir%;%PATH%

FOR %%f IN (*.sln) DO %%f

