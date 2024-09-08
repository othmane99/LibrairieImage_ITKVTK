//#include <vtkConeSource.h>
//#include <vtkPolyDataMapper.h>
//#include <vtkActor.h>
//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkSmartPointer.h>
//#include <vtkProperty.h>
//#include <vtkMath.h>
//#include <random>
//#include "cone_source.h"
//#include<vtktransform.h>
////#include <vtkPosition.h>
//
//
//void Observer::Execute(vtkObject* caller, unsigned long, void*)
//{
//	vtkRenderWindowInteractor* interactor = vtkRenderWindowInteractor::SafeDownCast(caller);
//	static bool add{ true };
//	const int resMax{ 100 }, resMin{ 4 };
//
//	// Test if 'r' is pressed
//	if (interactor->GetKeyCode() == 'r')
//	{
//		std::cout << "r is pressed";
//		int res{ coneSource->GetResolution() };
//		
//			if (add)
//			{ 
//				res++;
//				if (res > resMax) add = false;
//			}
//
//			else 
//			{ 
//				res--;
//				if (res < resMin) add = true;
//			}
//			coneSource->SetResolution(res);
//	}
//
//
//		//coneSource->SetResolution(coneSource->GetResolution() + 1);
//
//	
//}
//
//
//int main() {
//
//	//create cone source
//	vtkSmartPointer<vtkConeSource> coneSource = vtkConeSource::New();
//	coneSource->SetRadius(1.0);
//	coneSource->SetHeight(3.0);
//	coneSource->SetResolution(44.0);
//
//	//create a cone mapper 
//	vtkSmartPointer<vtkPolyDataMapper> coneMaper = vtkPolyDataMapper::New();
//	coneMaper->SetInputConnection(coneSource->GetOutputPort());
//
//	//renderer
//
//	vtkSmartPointer<vtkRenderer> renderer = vtkRenderer::New();
//	renderer->SetBackground(0.1, 0.2, 0.4);
//
//	double radius{ coneSource->GetRadius() };
//
//	// vtk actor
//	for (int i{}; i < 8; i++) {
//		for (int j{}; j < 8; j++) {
//			//vtkSmartPointer<vtkActor> coneActor = vtkActor::New();
//			vtkNew<vtkActor> coneActor;
//			coneActor->SetMapper(coneMaper);
//			coneActor->SetPosition(0, i*2*radius,2*radius*j);
//			coneActor->GetProperty()->SetColor(vtkMath::Random(0.0, 1), vtkMath::Random(0.0, 1), vtkMath::Random(0.0, 1));
//
//			renderer->AddActor(coneActor);
//		}
//	}
//
//	//create a render window
//	vtkSmartPointer<vtkRenderWindow> window = vtkRenderWindow::New();
//	window->AddRenderer(renderer);
//
//	//creat a render window interactor
//	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkRenderWindowInteractor::New();
//	interactor->SetRenderWindow(window);
//	
//	
//	
//	//Add cone source to observer 
//	vtkSmartPointer<Observer> observer = vtkSmartPointer<Observer>::New();
//	observer->SetConeSource(coneSource);
//	interactor->AddObserver(vtkCommand::KeyPressEvent, observer);
//	
//
//	/* Add an observer to the render window interactor
//	vtkSmartPointer<Observer> observer = vtkSmartPointer<Observer>::New();
//	observer->SetConeSource(coneSource);
//	interactor->AddObserver(vtkCommand::KeyPressEvent, observer);*/
//
//	interactor->Initialize();
//	interactor->Start();
//
//
//
//
//	return 0;
//}

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkCallbackCommand.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkAbstractTransform.h>
#include <vtkCamera.h>
#include <vtkMatrix3x3.h>
#include <vtkInteractorObserver.h>
#include <vtkInteractorStyle.h>

// Définir la fonction de rappel du minuteur
void FonctionRappelMinuteur(vtkObject* appelant, long eventId, void* clientData, void* callData);

// Créer un acteur de sphère représentant la boule
vtkSmartPointer<vtkActor> CreerActeurBoule();

// Initialiser la scène
void InitialiserScene(vtkSmartPointer<vtkRenderer> rendu);

// Variables globales pour la position et la direction de la boule
double positionBoule[3] = { 0.0, 0.0, 0.0 };
double directionBoule[3] = { 0.0, 0.0, 0.0 };

int main(int argc, char* argv[])
{
    // Créer un rendu
    vtkSmartPointer<vtkRenderer> rendu = vtkSmartPointer<vtkRenderer>::New();

    // Initialiser la scène
    InitialiserScene(rendu);

    // Créer une fenêtre de rendu
    vtkSmartPointer<vtkRenderWindow> fenetreRendu = vtkSmartPointer<vtkRenderWindow>::New();
    fenetreRendu->AddRenderer(rendu);

    // Créer un interacteur de fenêtre de rendu
    vtkSmartPointer<vtkRenderWindowInteractor> interacteurFenetreRendu =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interacteurFenetreRendu->SetRenderWindow(fenetreRendu);

    // Créer un minuteur pour animer le déplacement de la boule
    vtkSmartPointer<vtkCallbackCommand> rappelMinuteur =
        vtkSmartPointer<vtkCallbackCommand>::New();
    rappelMinuteur->SetCallback(FonctionRappelMinuteur);
    interacteurFenetreRendu->AddObserver(vtkCommand::TimerEvent, rappelMinuteur);

    // Démarrer le minuteur avec un intervalle de 10 ms
    interacteurFenetreRendu->CreateRepeatingTimer(10);

    // Démarrer l'interaction
    fenetreRendu->Render();
    interacteurFenetreRendu->Start();

    return 0;
}

void FonctionRappelMinuteur(vtkObject* appelant, long eventId, void* clientData, void* callData)
{
    // Mettre à jour la position de la boule en fonction de la direction actuelle
    positionBoule[0] += directionBoule[0];
    positionBoule[1] += directionBoule[1];
    positionBoule[2] += directionBoule[2];

    // Gérer les rebonds sur les bords du billard
    if (positionBoule[0] < -1.0 || positionBoule[0] > 1.0)
    {
        directionBoule[0] *= -1.0;
    }
    if (positionBoule[1] < -1.0 || positionBoule[1] > 1.0)
    {
        directionBoule[1] *= -1.0;
    }

    // Code pour mettre à jour la position visuelle de la boule va ici

    // Mettre à jour la position visuelle de la boule
    // ...

    // Mettre à jour la vue
    appelant->GetRenderWindow()->Render();
}

vtkSmartPointer<vtkActor> CreerActeurBoule()
{
    // Créer une source de sphère
    vtkSmartPointer<vtkSphereSource> sourceSphère = vtkSmartPointer<vtkSphereSource>::New();
    sourceSphère->SetRadius(0.1);  // Définir le rayon de la boule

    // Créer un mappage
    vtkSmartPointer<vtkPolyDataMapper> mappage = vtkSmartPointer<vtkPolyDataMapper>::New();
    mappage->SetInputConnection(sourceSphère->GetOutputPort());

    // Créer un acteur
    vtkSmartPointer<vtkActor> acteur = vtkSmartPointer<vtkActor>::New();
    acteur->SetMapper(mappage);
    acteur->GetProperty()->SetColor(1.0, 0.0, 0.0);  // Définir la couleur de la boule (par exemple, rouge)

    return acteur;
}

void InitialiserScene(vtkSmartPointer<vtkRenderer> rendu)
{
    // Définir la couleur de l'arrière-plan
    rendu->SetBackground(0.0, 0.6, 0.0);  // Vert

    // Créer et ajouter l'acteur de la boule au rendu
    vtkSmartPointer<vtkActor> acteurBoule = CreerActeurBoule();
    rendu->AddActor(acteurBoule);

    // Définir une direction aléatoire pour le mouvement initial de la boule
    vtkMath::RandomSeed(time(NULL));
    directionBoule[0] = vtkMath::Random(-1.0, 1.0);
    directionBoule[1] = vtkMath::Random(-1.0, 1.0);
    directionBoule[2] = 0.0;  // Mouvement 2D (en surface)

    // Normaliser la direction
    double magnitude = vtkMath::Norm(directionBoule);
    if (magnitude > 0.0)
    {
        directionBoule[0] /= magnitude;
        directionBoule[1] /= magnitude;
        directionBoule[2] /= magnitude;
    }

    // Positionner la boule au centre du billard
    positionBoule[0] = 0.0;
    positionBoule[1] = 0.0;
    positionBoule[2] = 0.0;

    // Ajouter tout autre code d'initialisation nécessaire ici
}
