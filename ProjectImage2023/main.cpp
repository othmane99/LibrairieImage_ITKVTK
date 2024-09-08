#include<iostream>
#include<itkImage.h>
#include<itkImageFileReader.h>
#include<itkCastImageFilter.h>
#include<itkSobelEdgeDetectionImageFilter.h>
#include<itkImageFileWriter.h>
#include<itkRescaleIntensityImageFilter.h>


#include<iostream>
#include<vtkSmartPointer.h>
#include<vtkJPEGReader.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>

#include "itkImageToVTKImageFilter.h"
#include "itkImageToVTKImageFilter.hxx"


int main(int argc, char* argv[])
{

	using UCharPixelType = unsigned char;
	using FloatPixelType = float;
	using UCharImageType = itk::Image<UCharPixelType, 2>;
	using FloatImageType = itk::Image<FloatPixelType, 2>;

	// image file reader

	using FileReaderType = itk::ImageFileReader<UCharImageType>;
	FileReaderType::Pointer reader = FileReaderType::New();
	reader->SetFileName(argv[1]);

	//  cast image uchar  float

	using UChar2FloatCastFilterType = itk::CastImageFilter < UCharImageType, FloatImageType>;
	UChar2FloatCastFilterType::Pointer uChar2FloatFilter = UChar2FloatCastFilterType::New();
	uChar2FloatFilter->SetInput(reader->GetOutput());

	//sobel detection
	using SobelFilterType = itk::SobelEdgeDetectionImageFilter<FloatImageType, FloatImageType>;
	SobelFilterType::Pointer sobelFilter = SobelFilterType::New();
	sobelFilter->SetInput(uChar2FloatFilter->GetOutput());
	//  image cast
	using Float2UCharCastFilterType = itk::CastImageFilter<FloatImageType, UCharImageType>;
	Float2UCharCastFilterType::Pointer float2UCharFilter = Float2UCharCastFilterType::New();
	float2UCharFilter->SetInput(sobelFilter->GetOutput());

	using RescaleFilterType = itk::RescaleIntensityImageFilter<FloatImageType, UCharImageType>;
	RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
	rescaleFilter->SetInput(sobelFilter->GetOutput());
	rescaleFilter->SetOutputMinimum(0);
	rescaleFilter->SetOutputMaximum(255);

	//
	/*
	using FileWriterType = itk::ImageFileWriter<UCharImageType>;
	FileWriterType::Pointer writer = FileWriterType::New();
	writer->SetInput(float2UCharFilter->GetOutput());
	writer->SetFileName("result.jpg");
	writer->Update();
	*/


	// rescaleFilter -> GetOutput() est une itk::Image
	// actor -> SetInputData() prend une vtkImage en parametre
	// La conversion se fait avec une itkImageTOVTKImageFilter

	using ConversionFilterType = itk::ImageToVTKImageFilter<UCharImageType>;
	ConversionFilterType::Pointer conversionFilter = ConversionFilterType::New();
	conversionFilter->SetInput(rescaleFilter->GetOutput());

	//Pour finaliser le pipeline 
	conversionFilter->Update();


	//create an image ACtor
	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
	actor->SetInputData(conversionFilter->GetOutput());
	//actor->SetInputData(reader->GetOutput());



	vtkSmartPointer<vtkRenderer> renderer = vtkRenderer::New();
	renderer->SetBackground(0.1, 0.2, 0.4);
	renderer->AddActor(actor);


	//create a render window
	vtkSmartPointer<vtkRenderWindow> window = vtkRenderWindow::New();
	window->AddRenderer(renderer);


	//creat a render window interactor
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkRenderWindowInteractor::New();
	interactor->SetRenderWindow(window);


	/* image interactor style
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkRenderWindowInteractor::New();
	interactor->SetRenderWindow(window)*/


	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer <vtkInteractorStyleImage>::New();
	interactor->SetInteractorStyle(style);

	interactor->Initialize();
	interactor->Start();



	return 0;
}

/*

	using FileWriterType = itk::ImageFileWriter<UCharImageType>;
	FileWriterType::Pointer writer = FileWriterType::New();
	writer->SetInput(float2UCharFilter->GetOutput());
	writer->SetFileName("result.jpg");
	writer->Update();






	*/