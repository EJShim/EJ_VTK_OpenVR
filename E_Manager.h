#pragma once
#include "K_IGSTK_Header.h"

#include <vtkSmartPointer.h>
#include <vtkOpenVRCamera.h>
#include <vtkOpenVRRenderer.h>
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include "E_InteractorVR.h"


#include "E_CuttingPlane.h"


class E_Manager
{
public:
	E_Manager();
	~E_Manager();

private:
	static E_Manager* m_instance;
	static void Destroy() { delete m_instance; m_instance = 0; }

public:
	static E_Manager* Mgr();

public:
	void Initialize();
	void ClearMemory();

	void Start();


protected:
	std::string ReadStringFromFile(const char* filepath);
	void AddInitialMeshes();
public:
	///Get Renderer
	vtkSmartPointer<vtkRenderer> GetRenderer() { return m_renderer; }

	///TEST
	void UpdateInstructionPosition(double x, double y, double z);

	///Cutting Plane Actions
	void AddCuttingPlane(int idx, double* ori);
	void RemoveCuttingPlane(int idx = 0);
	void UpdateCuttingPlaneTransform(int idx, double *pos, double *ori);

protected:
	vtkSmartPointer<vtkOpenVRCamera> m_camera;
	vtkSmartPointer<vtkOpenVRRenderer> m_renderer;
	vtkSmartPointer<vtkOpenVRRenderWindow> m_renderWindow;
	vtkSmartPointer<E_InteractorVR> m_interactor;

public:
	vtkSmartPointer<vtkActor> m_mandible;
	vtkSmartPointer<vtkActor> m_fibula;
	
	vtkSmartPointer<E_CuttingPlane> m_cuttingPlane[2];





};

