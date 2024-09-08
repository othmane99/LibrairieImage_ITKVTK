#include <vtkCommand.h>
#include <vtkConeSource.h>
class Observer : public vtkCommand {
public:
	static Observer* New()
	{
		return new Observer;
	}
	void Execute(vtkObject*, unsigned long, void*);
	void SetConeSource(vtkConeSource* coneSource) { this->coneSource = coneSource; }
private:
	vtkConeSource* coneSource;
};