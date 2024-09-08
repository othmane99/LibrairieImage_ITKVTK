#ifndef __Ball_h
#define __Ball_h


#endif

#include <vtkCommand.h>
#include <vtkConeSource.h>
class Observer : public vtkCommand {
public:
	static Observer* New()
	{
		return new Observer;
	}
	void Execute(vtkObject*, unsigned long, void*);
	void SetSphereSource(vtkSphereSource* sphereSource) 
	{ 
		this->sphereSource = sphereSource; 
	}
private:
	vtkSphereSource* sphereSource;
};