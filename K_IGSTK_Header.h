//////////////////////////////////////////////////////////////////////////
/// 
/// \file	K_IGSTK_Header.h
/// \author	hccho
/// 
/// API의 binary interface를 지정하기 위한 Macro 정의
/// 
//////////////////////////////////////////////////////////////////////////

#ifndef K_IGSTK_HEADER
#define K_IGSTK_HEADER

#include <vtkConfigure.h>
#include <vtkAutoInit.h>
#define vtkRenderingCore_AUTOINIT 4(vtkRenderingOpenGL2,vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingContextOpenGL2)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL2)


#endif


