#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkMath.h>
#include <random>


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
};



int main(int , char*[]) {
	vtkSmartPointer<vtkSphereSource> sphere = vtkSphereSource::New();
	sphere->SetRadius(15.0);
	sphere->SetCenter(0.0, 0.0, 0.0);
	sphere->SetThetaResolution(60.0);
	sphere->SetPhiResolution(15.0);
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkPolyDataMapper::New();
	mapper->SetInputConnection(sphere->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkActor::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0.1,0.2,0.3);

	vtkSmartPointer<vtkRenderer> render=vtkRenderer::New();
	render->SetBackground(255,255,255);
	render->AddActor(actor);
	

	vtkSmartPointer<vtkRenderWindow> winrender = vtkRenderWindow::New();
	winrender->AddRenderer(render);
	winrender->SetWindowName("Timer");

	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkRenderWindowInteractor::New();
	interactor->SetRenderWindow(winrender);

	winrender->Render();
	
	vtkNew<vtkTimerCallback> cb;
	interactor->AddObserver(vtkCommand::TimerEvent, cb);

	int timerId = interactor->CreateRepeatingTimer(100);
	std::cout << "timerId: " << timerId << std::endl;
	// Destroy the timer when maxCount is reached.
	cb->maxCount = 100;
	cb->timerId = timerId;



	interactor->Initialize();
	interactor->Start();









}