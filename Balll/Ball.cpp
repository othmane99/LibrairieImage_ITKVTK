#include <vtkActor.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPlaneSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include "Ball.h"
#include <vtkSphereSource.h>

#include <vtkTexture.h>
#include <vtkTextureMapToSphere.h>
#include <vtkTexturedSphereSource.h>
#include <vtkTransformTextureCoords.h>


#include<vtkPNGReader.h>
#include <array>


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
		pas = 0.01;
		sphere->SetPosition(plane->GetCenter()[0], plane->GetCenter()[1], sphereRadius);
		double positionX = sphere->GetPosition()[0];
		step = positionX +pas;

		//lunch the timer 
		if (vtkCommand::TimerEvent == eventId)
		{
			++this->TimerCount;

			std::cout << this->TimerCount << std::endl;
			//2 iterations as period
			if (this->TimerCount == 10)
			{

				step = step + pas;
				sphere->SetPosition(step, 0, sphereRadius);

				iren->Render();
				if(positionX>=plane->GetLength()){ sphere->SetPosition(0, 0, sphereRadius); } //En cas de depassement du plan retour au centre 

				this->TimerCount = 0;			

				std::cout << "Ball is in the position : " << positionX <<  "\n";//Just for tests
				iren->Render();


			}
		}
		
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

	int maxCount = 10;
	int timerId = -1;
	vtkActor* sphere = nullptr;
	vtkActor* plane = nullptr;
	double sphereRadius;
	double pas;
	double step;


};

void Observer::Execute(vtkObject* caller, unsigned long, void*)
{
	vtkRenderWindowInteractor* interactor = vtkRenderWindowInteractor::SafeDownCast(caller);
	//static bool add{ true };
	//const int resMax{ 100 }, resMin{ 4 };

	// Test if 'a' is pressed
	if (interactor->GetKeyCode() == 'a')
	{
		std::cout << "a is pressed";
		// Augmenter le step en passant une variable globale

	}
	// Test if 'd' is pressed
	if (interactor->GetKeyCode() == 'd')
	{
		std::cout << "d is pressed";
		// The invers by passing a negative step using a global variable

	}
}




int main(int argc, char* argv[])
{
	vtkNew<vtkNamedColors> colors;
	/*
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << "basketball-texture.png"
			<< " [translate]" << std::endl;
		return EXIT_FAILURE;
	}
	double translate[3];
	if (argc > 2)
	{
		translate[0] = atof(argv[2]);
	}
	else
	{
		translate[0] = 0.0;
	}
	translate[1] = 0.0;
	translate[2] = 0.0;
	std::cout << translate[0] << ", " << translate[1] << ", " << translate[2]
		<< "\n";

	*/
	





	// Set the background color.
	std::array<unsigned char, 4> bkg{ {26, 51, 77, 255} };
	colors->SetColor("BkgColor", bkg.data());

	// Create a plane horizental to z axis
	vtkNew<vtkPlaneSource> planeSource;
	planeSource->SetCenter(0.0, 0.0, 0.0);
	planeSource->SetNormal(0.0, 0.0, 1.0);
	planeSource->Update();

	// Create a sphere
	//vtkNew<vtkTexturedSphereSource> sphereSource;
	vtkNew<vtkSphereSource> sphereSource;
	sphereSource->SetRadius(0.1);
	// Make the surface smooth.
	sphereSource->SetPhiResolution(100);
	sphereSource->SetThetaResolution(100);
	//planeSource->GetCenter();
	double sphereRadius = sphereSource->GetRadius();
	sphereSource->SetCenter(0, 0, sphereRadius);
	
	//recieve the png
	vtkNew<vtkPNGReader> PNGReader;
	PNGReader->SetFileName("C:\\Users\\oboum\\source\\repos\\Ball\\basketball-texture.png");

	vtkNew<vtkTexture> texture;
	texture->SetInputConnection(PNGReader->GetOutputPort());

	vtkNew<vtkTextureMapToSphere> texturesphere;
	texturesphere->SetInputConnection(sphereSource->GetOutputPort());

	//planSource mapper
	vtkNew<vtkPolyDataMapper> mapper1;
	mapper1->SetInputConnection(planeSource->GetOutputPort());
	//planeSource actor
	vtkNew<vtkActor> actor1;
	actor1->SetMapper(mapper1);
	actor1->GetProperty()->SetColor(colors->GetColor3d("Banana").GetData());
	//TextureSphere mapper
	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(texturesphere->GetOutputPort());
	//TextureSphere actor
	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	actor->SetTexture(texture);
	

	

	vtkNew<vtkRenderer> renderer;
	renderer->AddActor(actor1);
	renderer->AddActor(actor);
	renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());

	vtkNew<vtkRenderWindow> renderWindow;
	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("BALL WINDOW");

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
	cb->sphere = actor;
	cb->plane = actor1;
	cb->sphereRadius = sphereRadius;

	interactor->Start();

	vtkSmartPointer<Observer> observer = vtkSmartPointer<Observer>::New();


	//Add cone source to observer 
	observer->SetSphereSource(sphereSource);
	interactor->AddObserver(vtkCommand::KeyPressEvent, observer);



	return EXIT_SUCCESS;
}