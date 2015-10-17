// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"

#include "OculusKinectGameMode.generated.h"


UCLASS(minimalapi)
class AOculusKinectGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AOculusKinectGameMode();
	void UpdateKinectST();
	HRESULT StartKinectST();


private:
	HWND                    m_hWnd;

	bool                    m_bSeatedMode;

	// Current Kinect
	INuiSensor*             m_pNuiSensor;

	// Skeletal drawing
	ID2D1HwndRenderTarget*   m_pRenderTarget;;
	D2D1_POINT_2F            m_Points[NUI_SKELETON_POSITION_COUNT];

	// Direct2D
	ID2D1Factory*           m_pD2DFactory;

	HANDLE                  m_pSkeletonStreamHandle;
	HANDLE                  m_hNextSkeletonEvent;
};



