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
	void SetSphereSource(vtkSphereSource* sphereSource) { this->sphereSource = sphereSource; }
private:
	vtkSphereSource* sphereSource;
};

//#ifndef BALL_H
//#define BALL_H
//
//#include <vtkSmartPointer.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkRenderWindow.h>
//#include <vtkActor.h>
//
//class Ball
//{
//public:
//    Ball();
//
//    // M�thodes pour cr�er la sc�ne et d�marrer l'interaction
//    void CreateScene();
//    void StartInteraction();
//
//private:
//    vtkSmartPointer<vtkRenderWindowInteractor> interactor;
//
//    // M�thodes pour cr�er le plan et la sph�re textur�e
//    void CreatePlane();
//    void CreateTexturedSphere();
//
//    
//
//    // M�thode pour g�rer l'�v�nement de minuterie
//    void TimerCallback(vtkObject* caller, long unsigned int vtkNotUsed, void* clientData);
//    //static void TimerCallback(vtkObject* caller, long unsigned int eventId, void* clientData);
//    //static void TimerCallback(vtkObject* caller, unsigned long eventId, void* clientData, void* callData);
//
//
//    // M�thodes pour g�rer les touches "a" et "d"
//    void Accelerate();
//    void Decelerate();
//
//    vtkSmartPointer<vtkActor> planeActor;
//    vtkSmartPointer<vtkActor> sphereActor;
//
//    double sphereRadius;
//    double spherePosition[3];
//    double sphereRotationAngle;
//    double sphereRotationSpeed;
//};
//
//#endif // BALL_H
