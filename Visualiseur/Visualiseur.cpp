#include<iostream>
#include<vtkSmartPointer.h>
#include<vtkJPEGReader.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>
int main(int argc, char* argv[])
	{

		std::cout << "Nombre d'argument de la ligne de commande : " << argc << '\n';
		std::cout << "Nom de l'éxecutable : " << argv[0] << '\n';
		for (int i{1}; i < argc; ++i)
		{

			std::cout<<"Argument"<<i<<" : "<<argv[i]<<'\n' ;
		}
		return 0;

		//  open a JPEG image file 
		vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
		reader->SetFileName(argv[1]);
		reader->Update();

		//create an image ACtor
		vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
		actor->SetInputData(reader->GetOutput());

		vtkSmartPointer<vtkRenderer> renderer = vtkRenderer::New();
		renderer->SetBackground(0.1, 0.2, 0.4);
		renderer->AddActor(actor);


		//create a render window
		vtkSmartPointer<vtkRenderWindow> window = vtkRenderWindow::New();
		window->AddRenderer(renderer);
		

		//creat a render window interactor
		vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkRenderWindowInteractor::New();
		interactor->SetRenderWindow(window); 
			

			/* image interactor style
		vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkRenderWindowInteractor::New();
		interactor->SetRenderWindow(window)*/


		vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer <vtkInteractorStyleImage>::New();
		interactor->SetInteractorStyle(style);

		interactor->Initialize();
		interactor->Start();





	}