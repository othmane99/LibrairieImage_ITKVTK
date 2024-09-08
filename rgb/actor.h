#include <vtkCommand.h>

class Observer : public vtkCommand {
public:
	static Observer* New()
	{
		return new Observer;
	}
	void Execute(vtkObject*, unsigned long, void*);
	void Actor(vtkImageActor* actor) { this->actor = actor; }
private:
	vtkImageActor* actor;
};