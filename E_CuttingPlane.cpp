#include "E_CuttingPlane.h"

#include <vtkPlaneSource.h>
#include <vtkProperty.h>
#include <vtkPolyDAtaMapper.h>
#include <vtkCutter.h>
#include <vtkVector.h>


E_CuttingPlane::E_CuttingPlane()
{
	Initialize();
}


E_CuttingPlane::~E_CuttingPlane()
{

}

void E_CuttingPlane::Initialize()
{
	// Create a plane
	this->SetOrigin(0.0, 0.0, -0.5);
	this->SetNormal(1.0, 0.0, 0.0);

	vtkSmartPointer<vtkPlaneSource> planeSource = vtkSmartPointer<vtkPlaneSource>::New();
	planeSource->SetCenter(0.0, 0.0, -0.5);
	planeSource->SetNormal(1.0, 0.0, 0.0);
	planeSource->Update();

	vtkPolyData* plane = planeSource->GetOutput();

	// Create a mapper and actor
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(plane);

	m_actor = vtkSmartPointer<vtkActor>::New();
	m_actor->SetMapper(mapper);
	m_actor->SetScale(0.2, 0.2, 0.2);
	m_actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
	m_actor->GetProperty()->SetOpacity(0.2);
}

void E_CuttingPlane::Start(bool inv, double *ori)
{
	double n[3] = { 1.0, 0.0, 0.0 };
	if (!inv) n[0] = -1.0;

	m_actor->SetOrientation(n);
	m_actor->RotateWXYZ(ori[0], ori[1], ori[2], ori[3]);

	this->SetNormal(n);


	vtkSmartPointer<vtkTransform> trans = vtkSmartPointer<vtkTransform>::New();
	trans->RotateWXYZ(ori[0], ori[1], ori[2], ori[3]);
	trans->Update();

	double *dn;
	dn = trans->TransformDoubleNormal(n);

	this->SetNormal(dn);
}

void E_CuttingPlane::Update(double* pos, double *ori)
{
	//Set Position
	this->SetOrigin(pos[0], pos[1], pos[2]);
	m_actor->SetPosition(pos[0], pos[1], pos[2]);

	//Set Orientation
	m_actor->RotateWXYZ(ori[0], ori[1], ori[2], ori[3]);

	// Set Normal Plane
	vtkSmartPointer<vtkTransform> trans = vtkSmartPointer<vtkTransform>::New();
	trans->RotateWXYZ(ori[0], ori[1], ori[2], ori[3]);
	trans->Update();

	double n[3];
	this->GetNormal(n);

	double *dn;
	dn = trans->TransformDoubleNormal(n);
	this->SetNormal(dn);
}


void E_CuttingPlane::Remove()
{

}