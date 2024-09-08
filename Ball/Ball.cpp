#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNamedColors.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkAppendPolyData.h>
#include <vtkCamera.h>
#include<iostream>
#include<itkImage.h>
#include<itkImageFileReader.h>
#include<itkCastImageFilter.h>
#include<itkSobelEdgeDetectionImageFilter.h>
#include<itkImageFileWriter.h>
#include<itkImageRegionIterator.h>
#include<itkImageRegionIteratorWithIndex.h>
#include<itkNeighborhood.h>
#include<itkNeighborhoodInnerProduct.h>
#include<itkImageRegionIterator.h>
#include<vtkDiskSource.h>
#include"itkImageToVTKImageFilter.h"
#include<vtkImageActor.h>
#include <list>
#include <vector>
#include <vtkCommand.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>





class vtkTimerCallback : public vtkCommand
{
public:
	vtkTimerCallback() = default;

	static vtkTimerCallback* New()
	{
		vtkTimerCallback* cb = new vtkTimerCallback;
		cb->TimerCount = 0;
		return cb;
	}

	virtual void Execute(vtkObject* caller, unsigned long eventId,
		void* vtkNotUsed(callData))
	{
		auto iren = dynamic_cast<vtkRenderWindowInteractor*>(caller);
		pas_x = sphere->GetPosition()[0];
		pas_y = sphere->GetPosition()[1];

		const double f_x = sphere->GetPosition()[0];
		const double f_y = sphere->GetPosition()[1];

		//lunch the timer 
		if (vtkCommand::TimerEvent == eventId)
		{
			++this->TimerCount;

			std::cout << this->TimerCount << std::endl;

			if (this->TimerCount == 1)
			{

				//postion[0] = sphere->GetCenter()[0];
				//postion[1] = sphere->GetCenter()[1];

				double x = radius * cos(angle);
				double y = radius * sin(angle);
				double Tol = std::sqrt(sphere->GetPosition()[0] * sphere->GetPosition()[0] + sphere->GetPosition()[1] * sphere->GetPosition()[1]);
				//sphere->SetPosition(x, y, 0);
				pas_x = pas_x + stepx;
				pas_y = pas_y + stepy;

				sphere->SetPosition(pas_x, pas_y, 0);

				iren->Render();
				cp = cp + 1;
				angle = theta * cp;
				if (Tol >= radius  /*sphere->GetCenter()[0] == radius || sphere->GetCenter()[1] == radius*/) {

					//stepx = stepx * (-1); stepy = stepy * (-1);
					sphere->SetPosition(width, height, 0);

				}
				if (angle >= 2 * pi) { cp = 0; }

				//double x = postion[0] + 3;
				//double x = postion[0] + 3;

				this->TimerCount = 0;///so we can replay 				

				std::cout << angle << "  " << theta << "\n";
				//if (postion[0] * postion[0] + postion[1] * postion[1] == radius * radius) {sphere->SetCenter(postion[0], postion[1], 0);}

				iren->Render();//refresh window


			}
		}
		//std::cout << this->TimerCount << std::endl;
		if (this->TimerCount >= this->maxCount)
		{
			if (this->timerId > -1)
			{
				iren->DestroyTimer(this->timerId);
			}
		}
	}

private:
	int TimerCount = 0;

public:

	int maxCount = 5;
	int timerId = -1;
	vtkActor* sphere = nullptr;
	double postion[2];
	double radius;
	//itk::Image<char, 2U>* img;
	//std::list<double *> my_list;
	std::vector<double[2]> my_list;
	std::vector<double> X;
	std::vector<double> Y;
	double width, height;
	const double theta = pi / 36;//5 deg
	const double pi = std::acos(-1);
	double angle = 0;
	int cp = 1;
	double pas_x;
	double pas_y;
	int stepx = 3; int stepy = 3;

};
/*class vtkTimerCallback : public vtkCommand
{
public:
	vtkTimerCallback() = default;

	static vtkTimerCallback* New()
	{
		vtkTimerCallback* cb = new vtkTimerCallback;
		cb->TimerCount = 0;
		return cb;
	}

	virtual void Execute(vtkObject* caller, unsigned long eventId,
		void* vtkNotUsed(callData))
	{
		if (vtkCommand::TimerEvent == eventId)
		{
			++this->TimerCount;
		}
		std::cout << this->TimerCount << std::endl;
		if (this->TimerCount >= this->maxCount)
		{
			auto iren = dynamic_cast<vtkRenderWindowInteractor*>(caller);
			if (this->timerId > -1)
			{
				iren->DestroyTimer(this->timerId);
			}
		}
	}

private:
	int TimerCount = 0;

public:
	int maxCount = 0;
	int timerId = -1;
};*/
int main(int, char* [])
{
	vtkNew<vtkNamedColors> colors;

	// Create a sphere
	vtkNew<vtkSphereSource> sphereSource;
	//sphereSource->SetCenter(0.0, 0.0, 0.0);
	sphereSource->SetRadius(5);
	// Make the surface smooth.
	sphereSource->SetPhiResolution(100);
	sphereSource->SetThetaResolution(100);

	//std::list<double *> my_list;
	std::vector<double> X;
	std::vector<double> Y;
	//
	using CharPixelType = char;
	using UCharPixelType = unsigned char;
	using FloatPixelType = float;
	using UCharImageType = itk::Image<UCharPixelType, 2>;
	using FloatImageType = itk::Image<FloatPixelType, 2>;


	//Allocation d'une nouvelle image de type UCharImageType

	UCharImageType::Pointer image = UCharImageType::New();
	//UCharImageType::IndexType start{ 0,0 };
	//UCharImageType::SizeType{ 512,512 };
	//UCharImageType::RegionType region(start, size;
	//image->SetRegions(region);
	image->SetRegions({ 215,215 });
	image->Allocate();

	UCharImageType::RegionType region{ image->GetLargestPossibleRegion() };
	UCharImageType::SizeType size{ region.GetSize() };
	UCharImageType::SizeValueType width{ size[0] }, height{ size[1] };
	const UCharImageType::SizeValueType radius{ width / 2 };
	sphereSource->SetCenter(width / 2, height / 2, 0);
	// Fil image with zero 

	image->FillBuffer(0);
	// draw white disque with an iterator with index 
	itk::ImageRegionIteratorWithIndex<UCharImageType> itIndex(image, image->GetLargestPossibleRegion());
	itIndex.GoToBegin();
	while (!itIndex.IsAtEnd())
	{

		//recuperer l'index
		UCharImageType::IndexType  index{ itIndex.GetIndex() };
		//
		double dx{ (double)index[0] - width / 2 }, dy{ (double)index[1] - height / 2 };
		if (dx * dx + dy * dy < radius * radius)
			itIndex.Set(255);

		//save the x&y in a list

		else if (dx * dx + dy * dy == radius * radius) {
			//double * x_y = { dx,dy };
			X.push_back(dx);
			Y.push_back(dy);


			std::cout << "coordonees de :" << dx << "     " << dy << "\n";
		}
		else
			itIndex.Set(0);
		++itIndex;
	}

	using ConversionFilterType = itk::ImageToVTKImageFilter<UCharImageType>;
	ConversionFilterType::Pointer conversionFilter = ConversionFilterType::New();
	conversionFilter->SetInput(image);
	conversionFilter->Update();




	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(sphereSource->GetOutputPort());

	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(colors->GetColor3d("Orange").GetData());

	vtkNew<vtkImageActor> actor1;
	actor1->SetInputData(conversionFilter->GetOutput());

	//actor1->GetProperty()->SetColor(colors->GetColor3d("Cornsilk").GetData());
	vtkNew<vtkRenderer> renderer;
	renderer->AddActor(actor);
	renderer->AddActor(actor1);
	renderer->SetBackground(colors->GetColor3d("DarkSlateGray").GetData());

	vtkNew<vtkRenderWindow> renderWindow;
	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("Timer");

	vtkNew<vtkRenderWindowInteractor> interactor;
	interactor->SetRenderWindow(renderWindow);

	// Initialize must be called prior to creating timer events.
	renderWindow->Render();
	interactor->Initialize();
	double centrex = actor->GetPosition()[0];
	double centrey = actor->GetPosition()[1];

	// Sign up to receive TimerEvent
	vtkNew<vtkTimerCallback> cb;
	interactor->AddObserver(vtkCommand::TimerEvent, cb);

	int timerId = interactor->CreateRepeatingTimer(100);
	std::cout << "timerId: " << timerId << std::endl;
	// Destroy the timer when maxCount is reached.
	cb->maxCount = 10;
	cb->timerId = timerId;
	cb->X = X;
	cb->Y = Y;
	cb->radius = radius;
	cb->sphere = actor;
	cb->width = centrex;
	cb->height = centrey;

	// Note: nothing is displayed in the render window.
	// Start the interaction and timer
	interactor->Start();

	return EXIT_SUCCESS;
}

