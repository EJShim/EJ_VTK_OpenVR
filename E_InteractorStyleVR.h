#pragma once

#include <vtkInteractorStyle3D.h>
#include "E_InteractorVR.h"

class E_InteractorStyleVR : public vtkInteractorStyle3D
{
public:
	static E_InteractorStyleVR* New() { return new E_InteractorStyleVR; }
	vtkTypeMacro(E_InteractorStyleVR, vtkInteractorStyle3D)
public:
	E_InteractorStyleVR();
	~E_InteractorStyleVR();

public:
	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();



protected:

	/// Tirgger Action
	void OnTriggerDown(int pointeridx);
	void OnTriggerRelease(int pointeridx);

	/// On Mouse Move
	void ControllerAction(int pointerIdx);

protected:
	/// Grip Color
	double m_prevColor[3];
};

