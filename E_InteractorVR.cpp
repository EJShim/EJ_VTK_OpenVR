#include "E_Manager.h"
#include "E_InteractorVR.h"
#include "E_InteractorStyleVR.h"
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkPropPicker3D.h>

E_InteractorVR::E_InteractorVR()
{
	vtkSmartPointer<E_InteractorStyleVR> interactorStyle = vtkSmartPointer<E_InteractorStyleVR>::New();
	this->SetInteractorStyle(interactorStyle);

	m_mode = MODE_NONE;
}


E_InteractorVR::~E_InteractorVR()
{
}



void E_InteractorVR::DoOneEvent(vtkOpenVRRenderWindow *renWin, vtkRenderer *ren)
{
	vr::IVRSystem *pHMD = renWin->GetHMD();

	if (!pHMD)
	{
		return;
	}

	vr::VREvent_t event;

	bool result = pHMD->PollNextEvent( &event, sizeof(vr::VREvent_t));

	if (result)
	{
		vr::TrackedDeviceIndex_t tdi = event.trackedDeviceIndex;

		// is it a controller button action?
		if (pHMD->GetTrackedDeviceClass(tdi) ==
			vr::ETrackedDeviceClass::TrackedDeviceClass_Controller &&
			(event.eventType == vr::VREvent_ButtonPress ||
				event.eventType == vr::VREvent_ButtonUnpress))
		{
			vr::ETrackedControllerRole role = pHMD->GetControllerRoleForTrackedDeviceIndex(tdi);

			this->UpdateTouchPadPosition(pHMD, tdi);

			// 0 = right hand 1 = left
			int pointerIndex =
				(role == vr::ETrackedControllerRole::TrackedControllerRole_RightHand ? 0 : 1);
			this->PointerIndexLookup[pointerIndex] = tdi;

			vr::TrackedDevicePose_t &tdPose = renWin->GetTrackedDevicePose(tdi);
			double pos[3];
			double wxyz[4];
			this->ConvertPoseToWorldCoordinates(ren, tdPose, pos, wxyz);

			// so even though we have world coordinates we have to convert them to
			// screen coordinates because all of VTKs picking code is currently
			// based on screen coordinates
			ren->SetWorldPoint(pos[0], pos[1], pos[2], 1.0);
			ren->WorldToDisplay();
			double *displayCoords = ren->GetDisplayPoint();

			this->SetEventPosition(displayCoords[0], displayCoords[1], pointerIndex);
			this->SetWorldEventPosition(pos[0], pos[1], pos[2], pointerIndex);
			this->SetPhysicalEventPosition(
				tdPose.mDeviceToAbsoluteTracking.m[0][3],
				tdPose.mDeviceToAbsoluteTracking.m[1][3],
				tdPose.mDeviceToAbsoluteTracking.m[2][3],
				pointerIndex);
			this->SetWorldEventOrientation(wxyz[0], wxyz[1], wxyz[2], wxyz[3], pointerIndex);
			this->SetPointerIndex(pointerIndex);

			if (event.eventType == vr::VREvent_ButtonPress)
			{
				if (event.data.controller.button == vr::EVRButtonId::k_EButton_Grip)
				{
					this->GripEvent();
				}
				if (event.data.controller.button == vr::EVRButtonId::k_EButton_Axis1)
				{
					this->TriggerEvent();
				}
				if (event.data.controller.button == vr::EVRButtonId::k_EButton_SteamVR_Touchpad)
				{
					this->PadEvent();
				}
				if (event.data.controller.button == vr::EVRButtonId::k_EButton_ApplicationMenu)
				{
					this->Done = true;
				}

			}
			if (event.eventType == vr::VREvent_ButtonUnpress)
			{
				if (event.data.controller.button == vr::EVRButtonId::k_EButton_Grip) {
					this->GripReleaseEvent();
				}
				if (event.data.controller.button == vr::EVRButtonId::k_EButton_Axis1)
				{
					this->TriggerReleaseEvent();
				}
				if (event.data.controller.button == vr::EVRButtonId::k_EButton_SteamVR_Touchpad)
				{
					this->PadReleaseEvent();
				}

			}
		}
	}
	else
	{
		// if pointers are down track the movement
		if (this->PointersDownCount)
		{
			

			for (int i = 0; i < VTKI_MAX_POINTERS; i++)
			{

				if (this->PointersDown[i])
				{
					
					this->UpdateTouchPadPosition(pHMD, static_cast<vr::TrackedDeviceIndex_t>(this->PointerIndexLookup[i]));
					vr::TrackedDevicePose_t &tdPose =
						renWin->GetTrackedDevicePose(static_cast<vr::TrackedDeviceIndex_t>(this->PointerIndexLookup[i]));
					double pos[3];
					double wxyz[4];
					this->ConvertPoseToWorldCoordinates(ren, tdPose, pos, wxyz);

			

					// so even though we have world coordinates we have to convert them to
					// screen coordinates because all of VTKs picking code is currently
					// based on screen coordinates
					ren->SetWorldPoint(pos[0], pos[1], pos[2], 1.0);
					ren->WorldToDisplay();
					double *displayCoords = ren->GetDisplayPoint();
					this->SetEventPosition(displayCoords[0], displayCoords[1], i);
					this->SetWorldEventPosition(pos[0], pos[1], pos[2], i);
					this->SetWorldEventOrientation(wxyz[0], wxyz[1], wxyz[2], wxyz[3], i);
					this->SetPhysicalEventPosition(
						tdPose.mDeviceToAbsoluteTracking.m[0][3],
						tdPose.mDeviceToAbsoluteTracking.m[1][3],
						tdPose.mDeviceToAbsoluteTracking.m[2][3],
						i);
				}
			}
			this->MouseMoveEvent();
		}
	}
	renWin->Render();
}

void E_InteractorVR::GripEvent()
{
	m_mode = MODE_PINCH;
	this->LeftButtonPressEvent();
}

void E_InteractorVR::GripReleaseEvent()
{
	this->LeftButtonReleaseEvent();
	m_mode = MODE_NONE;
}


void E_InteractorVR::TriggerEvent()
{
	m_mode = MODE_TRIGGER;
	this->RightButtonPressEvent();
}

void E_InteractorVR::TriggerReleaseEvent()
{
	this->RightButtonReleaseEvent();
}

void E_InteractorVR::PadEvent()
{
	m_mode = MODE_PAD;
	this->RightButtonPressEvent();
}

void E_InteractorVR::PadReleaseEvent()
{
	this->RightButtonReleaseEvent();
	m_mode = MODE_NONE;
}

void E_InteractorVR::MouseMoveEvent()
{
	E_InteractorStyleVR* interactorStyle = static_cast<E_InteractorStyleVR*>(GetInteractorStyle());
	interactorStyle->OnMouseMove();

	//Superclass::MouseMoveEvent();
}

void E_InteractorVR::LeftButtonPressEvent()
{
	if (!this->Enabled)
	{
		return;
	}

  // are we translating multitouch into gestures?
  if (this->RecognizeGestures)
  {
    if (!this->PointersDown[this->PointerIndex])
    {
      this->PointersDown[this->PointerIndex] = 1;
      this->PointersDownCount++;
    } 
  }

  this->InvokeEvent(vtkCommand::LeftButtonPressEvent, NULL);
}

void E_InteractorVR::RightButtonPressEvent()
{
	if (!this->Enabled)
	{
		return;
	}

	// are we translating multitouch into gestures?
	if (this->RecognizeGestures)
	{
		if (!this->PointersDown[this->PointerIndex])
		{
			this->PointersDown[this->PointerIndex] = 1;
			this->PointersDownCount++;
		}
	}

	this->InvokeEvent(vtkCommand::RightButtonPressEvent, NULL);
}