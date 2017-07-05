#pragma once

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkPlane.h>
#include <vtkTransform.h>


class E_CuttingPlane : public vtkPlane
{
public:
	static E_CuttingPlane* New() { return new E_CuttingPlane; }
	vtkTypeMacro(E_CuttingPlane, vtkPlane)
public:
	E_CuttingPlane();
	~E_CuttingPlane();

protected:
	void Initialize();


protected:
	vtkSmartPointer<vtkActor> m_actor;

public:
	///Update Cutting Plane Position and Orientation
	void Start(bool inv, double *ori);
	void Update(double *pos, double* ori);
	void Remove();

	vtkActor* GetActor() { return m_actor; }
};

