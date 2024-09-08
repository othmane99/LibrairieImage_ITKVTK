#include "itkRGBPixel.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRGBToLuminanceImageFilter.h" 
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkMath.h>
#include <random>
#include<vtktransform.h>
#include<iostream>
#include<itkImage.h>
#include<itkImageFileReader.h>
#include<itkCastImageFilter.h>
#include<itkSobelEdgeDetectionImageFilter.h>
#include<itkImageFileWriter.h>
#include"itkImageToVTKImageFilter.h"
#include<itkRescaleIntensityImageFilter.h>
#include<itkSobelEdgeDetectionImageFilter.h>
#include<itkSobelOperator.h>
#include<vtkInteractorStyleImage.h>
#include<vtkSmartPointer.h>
#include<vtkRenderer.h>
#include<vtkRenderWindow.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkImageActor.h>
#include "actor.h"
#include <vtkTransform.h>

void Observer::Execute(vtkObject* caller, unsigned long, void*)
{
	vtkRenderWindowInteractor* interactor = vtkRenderWindowInteractor::SafeDownCast(caller);
	static bool add{ true };
	const int resMax{ 100 }, resMin{ 4 };

	// Test if 'r' is pressed
	if (interactor->GetKeyCode() == 'x')
	{
		std::cout << "x is pressed";
		actor->RotateX(90);

	}

}


int main(int argc, char * argv[])
{


constexpr unsigned int Dimension = 2;
 
  using InputPixelType = itk::RGBPixel<unsigned char>;
  using InputImageType = itk::Image<InputPixelType, Dimension>;
  using OutputImageType = itk::Image<unsigned char, Dimension>;
 
 
  using ReaderType = itk::ImageFileReader<InputImageType>;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
 
 
  using FilterType = itk::RGBToLuminanceImageFilter<InputImageType, OutputImageType>;
 
  auto filter = FilterType::New();
 
  filter->SetInput(reader->GetOutput());
 

  using ConversionFilterType = itk::ImageToVTKImageFilter<OutputImageType>;
  ConversionFilterType::Pointer conversionFilter = ConversionFilterType::New();
  conversionFilter->SetInput(filter->GetOutput());
  conversionFilter->Update();


  vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
  actor->SetInputData(conversionFilter->GetOutput());



  vtkSmartPointer<vtkRenderer> renderer = vtkRenderer::New();
  renderer->SetBackground(0.1, 0.2, 0.4);
  renderer->AddActor(actor);

  


  //create a render window
  vtkSmartPointer<vtkRenderWindow> window = vtkRenderWindow::New();
  window->AddRenderer(renderer);


  //creat a render window interactor
  vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkRenderWindowInteractor::New();
  interactor->SetRenderWindow(window);
  vtkSmartPointer<Observer> observer = vtkSmartPointer<Observer>::New();


  //Add cone source to observer 
  observer->Actor(actor);
  interactor->AddObserver(vtkCommand::KeyPressEvent, observer);



  /* Add an observer to the render window interactor
  vtkSmartPointer<Observer> observer = vtkSmartPointer<Observer>::New();
  observer->SetConeSource(coneSource);
  interactor->AddObserver(vtkCommand::KeyPressEvent, observer);*/



  /*vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer <vtkInteractorStyleImage>::New();
  interactor->SetInteractorStyle(style);*/

  interactor->Initialize();
  interactor->Start();
 
 /* using WriterType = itk::ImageFileWriter<OutputImageType>;
 
  auto writer = WriterType::New();
 
  writer->SetInput(conversionFilter->GetOutput());
 
  writer->SetFileName("rgb.jpg");

  writer->Update();*/



return EXIT_SUCCESS;
}