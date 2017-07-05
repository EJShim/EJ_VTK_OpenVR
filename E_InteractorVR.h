#pragma once

#include <vtkOpenVRRenderWindowInteractor.h>

class E_InteractorVR : public vtkOpenVRRenderWindowInteractor
{
public:
	enum {MODE_NONE, MODE_TRIGGER, MODE_PAD, MODE_PINCH};
public:
	static E_InteractorVR *New() { return new E_InteractorVR; }
	vtkTypeMacro(E_InteractorVR, vtkOpenVRRenderWindowInteractor)
public:
	E_InteractorVR();
	~E_InteractorVR();


	

protected:

	void GripEvent();
	void GripReleaseEvent();

	void TriggerEvent();
	void TriggerReleaseEvent();
	
	void PadEvent();
	void PadReleaseEvent();

	virtual void DoOneEvent(vtkOpenVRRenderWindow *renWin, vtkRenderer *ren);
	
	virtual void MouseMoveEvent();
	virtual void LeftButtonPressEvent();
	virtual void RightButtonPressEvent();

protected:
	int m_mode;

public:
	int GetCurrentMode() { return m_mode; }
	void SetCurrentMode(int mode) { m_mode = mode; }

};

