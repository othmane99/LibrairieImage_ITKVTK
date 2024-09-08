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
//    // Méthodes pour créer la scène et démarrer l'interaction
//    void CreateScene();
//    void StartInteraction();
//
//private:
//    vtkSmartPointer<vtkRenderWindowInteractor> interactor;
//
//    // Méthodes pour créer le plan et la sphère texturée
//    void CreatePlane();
//    void CreateTexturedSphere();
//
//    
//
//    // Méthode pour gérer l'événement de minuterie
//    void TimerCallback(vtkObject* caller, long unsigned int vtkNotUsed, void* clientData);
//    //static void TimerCallback(vtkObject* caller, long unsigned int eventId, void* clientData);
//    //static void TimerCallback(vtkObject* caller, unsigned long eventId, void* clientData, void* callData);
//
//
//    // Méthodes pour gérer les touches "a" et "d"
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
