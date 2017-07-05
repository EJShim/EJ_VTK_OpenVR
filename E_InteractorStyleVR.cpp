#include "E_InteractorStyleVR.h"
#include "E_Manager.h"
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkPropPicker3D.h>
#include <vtkQuaternion.h>

E_InteractorStyleVR::E_InteractorStyleVR()
{
	for (int i = 0; i < 3; i++) {
		m_prevColor[i] = 1.0;
	}
}


E_InteractorStyleVR::~E_InteractorStyleVR()
{
}


void E_InteractorStyleVR::OnMouseMove()
{
	Superclass::OnMouseMove();

	E_InteractorVR* interactor = static_cast<E_InteractorVR*>(GetInteractor());

	//Get World Position
	int pointerCount = interactor->GetPointersDownCount();
	if (pointerCount> 1) { //Check if Multitouch
		for (int i = 0; i < pointerCount; i++) {
			ControllerAction(i);
		}
	}
	else {
		int pointeridx = interactor->GetPointerIndex();
		ControllerAction(pointeridx);
	}
}

void E_InteractorStyleVR::ControllerAction(int pointeridx)
{

	E_InteractorVR* interactor = static_cast<E_InteractorVR*>(GetInteractor());

	double *pos = interactor->GetWorldEventPosition(pointeridx);
	double *ori = interactor->GetWorldEventOrientation(pointeridx);

	double *wori = interactor->GetWorldEventOrientation(pointeridx);
	double *lwori = interactor->GetLastWorldEventOrientation(pointeridx);

	// compute the net rotation
	vtkQuaternion<double> q1, q2;
	q1.SetRotationAngleAndAxis(vtkMath::RadiansFromDegrees(lwori[0]), lwori[1], lwori[2], lwori[3]);
	q2.SetRotationAngleAndAxis(vtkMath::RadiansFromDegrees(wori[0]), wori[1], wori[2], wori[3]);
	q1.Conjugate();
	q2 = q2*q1;
	double axis[4];
	axis[0] = vtkMath::DegreesFromRadians(q2.GetRotationAngleAndAxis(axis + 1));


	//Check Mode
	if (interactor->GetCurrentMode() == E_InteractorVR::MODE_PAD) {

	}
	if (interactor->GetCurrentMode() == E_InteractorVR::MODE_TRIGGER) {
		E_Manager::Mgr()->UpdateCuttingPlaneTransform(pointeridx, pos, axis);
	}

	if (interactor->GetCurrentMode() == E_InteractorVR::MODE_PINCH) {
		vtkActor* target = InteractionPicker->GetActor();

		//If Target Actor exists..
		if (target != NULL) {

		}
	}
}

void E_InteractorStyleVR::OnLeftButtonDown()
{
	Superclass::OnLeftButtonDown();



	vtkActor* target = InteractionPicker->GetActor();
	if (target != NULL) {
		target->GetProperty()->GetColor(m_prevColor);
		target->GetProperty()->SetColor(1.0, 0.0, 0.0);
	}
}

void E_InteractorStyleVR::OnLeftButtonUp()
{
	vtkActor* target = InteractionPicker->GetActor();
	if (target != NULL) {
		target->GetProperty()->SetColor(m_prevColor);
	}

	Superclass::OnLeftButtonUp();
}

void E_InteractorStyleVR::OnRightButtonDown()
{
	E_InteractorVR* interactor = static_cast<E_InteractorVR*>(GetInteractor());
	int pointeridx = interactor->GetPointerIndex();
	int pointerCount = interactor->GetPointersDownCount();

	if (interactor->GetCurrentMode() == E_InteractorVR::MODE_PAD) {

	}
	if (interactor->GetCurrentMode() == E_InteractorVR::MODE_TRIGGER) {
		if (pointerCount > 1) {
			for (int i = 0; i < pointerCount; i++) {
				OnTriggerDown(i);
			}
		}
		else {
			OnTriggerDown(pointeridx);
		}
	}
}

void E_InteractorStyleVR::OnRightButtonUp()
{
	E_InteractorVR* interactor = static_cast<E_InteractorVR*>(GetInteractor());
	int pointeridx = interactor->GetPointerIndex();

	if (interactor->GetCurrentMode() == E_InteractorVR::MODE_PAD) {

	}
	if (interactor->GetCurrentMode() == E_InteractorVR::MODE_TRIGGER) {
		OnTriggerRelease(pointeridx);
	}
}


void E_InteractorStyleVR::OnTriggerDown(int pointeridx)
{
	//Add Cutting Plane
	E_InteractorVR* interactor = static_cast<E_InteractorVR*>(GetInteractor());

	double *wori = interactor->GetWorldEventOrientation(pointeridx);

	vtkQuaternion<double> q2;
	q2.SetRotationAngleAndAxis(vtkMath::RadiansFromDegrees(wori[0]), wori[1], wori[2], wori[3]);
	double axis[4];
	axis[0] = vtkMath::DegreesFromRadians(q2.GetRotationAngleAndAxis(axis + 1));

	E_Manager::Mgr()->AddCuttingPlane(pointeridx, axis);
}

void E_InteractorStyleVR::OnTriggerRelease(int pointeridx) 
{
	//Get Interactor
	E_InteractorVR* interactor = static_cast<E_InteractorVR*>(GetInteractor());

	//Get Pointer Count
	int pointerCount = interactor->GetPointersDownCount();

	if (pointerCount == 0) {
		//Remove Cutting Plane
		E_Manager::Mgr()->RemoveCuttingPlane(pointeridx);
		interactor->SetCurrentMode(E_InteractorVR::MODE_NONE);
	}
}