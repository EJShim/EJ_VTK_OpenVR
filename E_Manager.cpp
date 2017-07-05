#include "E_Manager.h"



#include <vtkSmartPointer.h>

#include <vtkActor.h>
#include <vtkOBJReader.h>
#include <vtkSTLReader.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <vtkCubeSource.h>
#include <vtkSphereSource.h>
#include <vtkVectorText.h>

#include <vtkProperty.h>
#include "E_InteractorStyleVR.h"



E_Manager::E_Manager()
{
	Initialize();
	AddInitialMeshes();
}


E_Manager::~E_Manager()
{
	ClearMemory();
}

E_Manager* E_Manager::m_instance;

E_Manager* E_Manager::Mgr()
{
	if (m_instance == NULL)
	{
		m_instance = new E_Manager();
		atexit(Destroy);
	}
	return m_instance;
}

void E_Manager::Initialize()
{
	//Initialize Renderer
	vtkSmartPointer<vtkOpenVRCamera> cam = vtkSmartPointer<vtkOpenVRCamera>::New();
	m_renderer = vtkSmartPointer<vtkOpenVRRenderer>::New();
	m_renderer->SetActiveCamera(cam);

	m_renderWindow= vtkSmartPointer<vtkOpenVRRenderWindow>::New();
	m_renderWindow->AddRenderer(m_renderer);

	m_interactor= vtkSmartPointer<E_InteractorVR>::New();
	m_renderWindow->SetInteractor(m_interactor);

	//
	m_renderer->SetBackground(1.0, 1.0, 1.0);
	m_renderer->SetBackground2(0.153, 0.217, 0.234);
	m_renderer->GradientBackgroundOn();

	for (int i = 0; i < 2; i++) {
		m_cuttingPlane[i] = NULL;
	}

}

void E_Manager::ClearMemory()
{

}

void E_Manager::Start()
{

	m_renderWindow->Render();
	m_interactor->Start();

}

void E_Manager::AddInitialMeshes()
{
	std::string filename = "D:\\Patient Data\\Data\\1_Mandible.stl";
	std::string VS = ReadStringFromFile("res\\test_vert.glsl");
	std::string FS = ReadStringFromFile("res\\test_frag.glsl");
	//std::cout << VS << std::endl;

	vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
	reader->SetFileName(filename.c_str());
	reader->Update();

	// Visualize
	vtkSmartPointer<vtkOpenGLPolyDataMapper> mapper = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
	//mapper->SetVertexShaderCode(VS.c_str());
	//mapper->SetFragmentShaderCode(FS.c_str());
	mapper->SetInputConnection(reader->GetOutputPort());

	m_mandible = vtkSmartPointer<vtkActor>::New();
	m_mandible->SetMapper(mapper);
	m_mandible->GetProperty()->SetColor(1.0, 0.0, 1.0);



	//Add Fibula
	filename = "D:\\Patient Data\\Data\\1_Fibula.stl";

	vtkSmartPointer<vtkSTLReader> reader2 = vtkSmartPointer<vtkSTLReader>::New();
	reader2->SetFileName(filename.c_str());
	reader2->Update();

	// Visualize
	vtkSmartPointer<vtkOpenGLPolyDataMapper> mapper2 = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
	mapper2->SetInputConnection(reader2->GetOutputPort());

	m_fibula = vtkSmartPointer<vtkActor>::New();
	m_fibula->SetMapper(mapper2);
	m_fibula->GetProperty()->SetColor(1.0, 1.0, 1.0);


	double *fibulaCenter;
	m_fibula->SetScale(0.002, 0.002, 0.002);
	fibulaCenter = m_fibula->GetCenter();
	m_fibula->SetPosition(-fibulaCenter[0] + 1.2, -fibulaCenter[1] + 0.7, -fibulaCenter[2]);



	double *mandibleCenter;
	m_mandible->SetScale(0.002, 0.002, 0.002);
	mandibleCenter = m_mandible->GetCenter();
	m_mandible->SetPosition(-mandibleCenter[0] + 1.2, -mandibleCenter[1] + 0.7, -mandibleCenter[2]);



	//Add Actor
	m_renderer->AddActor(m_mandible);
	m_renderer->AddActor(m_fibula);



}

void E_Manager::UpdateInstructionPosition(double x, double y, double z)
{

}


std::string E_Manager::ReadStringFromFile(const char* filepath)
{
	std::ifstream ifs(filepath);
	std::string content;

	content.assign((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

	return content;
}
	
void E_Manager::AddCuttingPlane(int idx /* = 0 */, double*ori)
{
	if (m_cuttingPlane[idx] == NULL) {
		m_cuttingPlane[idx] = vtkSmartPointer<E_CuttingPlane>::New();;
	}

	m_cuttingPlane[idx]->Start(idx, ori);
	m_mandible->GetMapper()->AddClippingPlane(m_cuttingPlane[idx]);
	m_renderer->AddActor(m_cuttingPlane[idx]->GetActor());


}

void E_Manager::RemoveCuttingPlane(int idx /* = 0 */)
{
	m_mandible->GetMapper()->RemoveClippingPlane(m_cuttingPlane[idx]);
	m_renderer->RemoveActor(m_cuttingPlane[idx]->GetActor());
}

void E_Manager::UpdateCuttingPlaneTransform(int idx, double *pos, double *ori)
{
	m_cuttingPlane[idx]->Update(pos, ori);
}