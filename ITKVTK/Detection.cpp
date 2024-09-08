/*
* Dans ce projet j'ai essayer d'utiliser itk puisqu'on l'a pas beaucoup utiliser dans le cours et tp,
* c'est pourquoi j'ai essayer d'utiliser pour une continuatiion d'une de mes parties d'un autre projet.
* mais j'ai eu plusieurs problemes bloquant.
* En fin j'ai essayer de visualiser lle resultat avec vtk, afin de mettre en oeuvre plus ou moins toutes 
* les parties vues au cours.
*/

#include<iostream>
#include<itkImage.h>
#include<itkImageFileReader.h>
#include<itkCastImageFilter.h>
#include<itkSobelEdgeDetectionImageFilter.h>
#include<itkImageFileWriter.h>
#include<itkRescaleIntensityImageFilter.h>
// Image segmentation includes
#include "itkConnectedThresholdImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"

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

#include <vector>

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"


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


#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkContourExtractor2DImageFilter.h"
#include "itkApproximateSignedDistanceMapImageFilter.h"
using namespace std;


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

	// here ///////////////////////////////////////////////////////////////////////////////////////
	using CastingFilterType =itk::CastImageFilter<FloatImageType, FloatImageType>;
	auto caster = CastingFilterType::New();

	using CurvatureFlowImageFilterType =itk::CurvatureFlowImageFilter<FloatImageType, FloatImageType>;
	auto smoothing = CurvatureFlowImageFilterType::New();

	using ConnectedFilterType =
		itk::ConnectedThresholdImageFilter<FloatImageType, FloatImageType>;

	//  Then we construct one filter of this class using the \code{New()}
	//  method.

	auto connectedThreshold = ConnectedFilterType::New();


	//  Now it is time to connect a simple, linear pipeline. A file reader is
	//  added at the beginning of the pipeline and a cast filter and writer
	//  are added at the end. The cast filter is required to convert
	//  \code{float} pixel types to integer types since only a few image file
	//  formats support \code{float} types.

	smoothing->SetInput(uChar2FloatFilter->GetOutput());
	connectedThreshold->SetInput(smoothing->GetOutput());
	caster->SetInput(smoothing->GetOutput());
	//writer->SetInput(caster->GetOutput());

	//  \code{CurvatureFlowImageFilter} requires a couple of parameters.
	//  The following are typical values for $2D$ images. However, these
	//  values may have to be adjusted depending on the amount of noise present
	//  in the input image.

	smoothing->SetNumberOfIterations(500);
	smoothing->SetTimeStep(0.125);


	//sobel detection
	using SobelFilterType = itk::SobelEdgeDetectionImageFilter<FloatImageType, FloatImageType>;
	SobelFilterType::Pointer sobelFilter = SobelFilterType::New();
	sobelFilter->SetInput(smoothing->GetOutput());

	//Countour///////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	using ApproximateSignedDistanceMapImageFilterType =
		itk::ApproximateSignedDistanceMapImageFilter<FloatImageType, FloatImageType>;
	ApproximateSignedDistanceMapImageFilterType::Pointer approximateSignedDistanceMapImageFilter =
		ApproximateSignedDistanceMapImageFilterType::New();
	approximateSignedDistanceMapImageFilter->SetInput(sobelFilter->GetOutput());
	approximateSignedDistanceMapImageFilter->SetInsideValue(255);
	approximateSignedDistanceMapImageFilter->SetOutsideValue(0);
	approximateSignedDistanceMapImageFilter->Update();

	using ContourExtractor2DImageFilterType = itk::ContourExtractor2DImageFilter<FloatImageType>;
	auto contourExtractor2DImageFilter = ContourExtractor2DImageFilterType::New();
	contourExtractor2DImageFilter->SetInput(approximateSignedDistanceMapImageFilter->GetOutput());
	contourExtractor2DImageFilter->SetContourValue(0);
	contourExtractor2DImageFilter->Update();

	std::cout << "There are " << contourExtractor2DImageFilter->GetNumberOfOutputs() << " contours" << std::endl;

	//auto tmp = ContourExtractor2DImageFilterType::New();
	//std::vector<int> countt;

	for (unsigned int i = 0; i < contourExtractor2DImageFilter->GetNumberOfOutputs(); ++i)
	{
		std::cout << "Contour " << i << ": " << std::endl;
		ContourExtractor2DImageFilterType::VertexListType::ConstIterator vertexIterator =
			contourExtractor2DImageFilter->GetOutput(i)->GetVertexList()->Begin();
		while (vertexIterator != contourExtractor2DImageFilter->GetOutput(i)->GetVertexList()->End())
		{
			std::cout << vertexIterator->Value() << std::endl;
			++vertexIterator;
			// Fill the vector by the size of all contours detected, which will provide us the 2 or 3 largest
			// contours detected 
			//++countt[i];
			// If the countt doesn't work well we can use an auto variable like tmp here
			//tmp = vertexIterator;

		}
		std::cout << std::endl;
		
	}
	//Return largest contour
	//cout << "\nMax Element = "<< *max_element(countt.begin(), countt.end());
	/*
	* I don't understand why it is not working.
	*/
	
	/*
	* The next step is to plot the 2 or 3 largest contours dected ant returned above
	*/

	//Image cast  float ucha
	using Float2UCharCastFilterType = itk::CastImageFilter<FloatImageType, UCharImageType>;
	Float2UCharCastFilterType::Pointer float2UCharFilter = Float2UCharCastFilterType::New();
	float2UCharFilter->SetInput(sobelFilter->GetOutput());

	//rescale filter output to cope with unsigned char
	using RescaleFilterType = itk::RescaleIntensityImageFilter<FloatImageType, UCharImageType>;
	RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
	rescaleFilter->SetInput(sobelFilter->GetOutput());
	rescaleFilter->SetOutputMinimum(0);
	rescaleFilter->SetOutputMaximum(255);

	
	//  conversion to vtk image
	using ConversionFilterType = itk::ImageToVTKImageFilter<UCharImageType>;
	ConversionFilterType::Pointer conversionFilter = ConversionFilterType::New();
	conversionFilter->SetInput(rescaleFilter->GetOutput());
	conversionFilter->Update();

	//create an image ACtor
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


	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer <vtkInteractorStyleImage>::New();
	interactor->SetInteractorStyle(style);

	interactor->Initialize();
	interactor->Start();

	return 0;
}



/*"C:\Users\oboum\source\repos\ITKVTK\45.jpg" "C:\Users\oboum\source\repos\ITKVTK\Result.jpeg"  368 368 150 180*/
