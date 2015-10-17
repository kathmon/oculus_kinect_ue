// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "OculusKinect.h"
#include "OculusKinectGameMode.h"
#include "OculusKinectHUD.h"
#include "OculusKinectCharacter.h"
#include <windows.h>
#include "AllowWindowsPlatformTypes.h"
#include <ole2.h>
#include <NuiApi.h>
#include <NuiSkeleton.h>
#include "HideWindowsPlatformTypes.h"


AOculusKinectGameMode::AOculusKinectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AOculusKinectHUD::StaticClass();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Pokusavam pokrenut kinect"));
	}


}
HRESULT AOculusKinectGameMode::StartKinectST()
{
	m_hNextSkeletonEvent = NULL;

	// Initialize m_pNuiSensor
	HRESULT hr = FindKinectSensor();
	if (SUCCEEDED(hr))
	{
		// Initialize the Kinect and specify that we'll be using skeleton
		hr = m_pNuiSensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);
		if (SUCCEEDED(hr))
		{
			// Create an event that will be signaled when skeleton data is available
			m_hNextSkeletonEvent = CreateEventW(NULL, TRUE, FALSE, NULL);

			// Open a skeleton stream to receive skeleton data
			hr = m_pNuiSensor->NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, 0);
		}
	}
	return hr;
}

// Call UpdateKinectST on each iteration of the application's update loop.
void AOculusKinectGameMode::UpdateKinectST()
{
	// Wait for 0ms, just quickly test if it is time to process a skeleton
	if (WAIT_OBJECT_0 == WaitForSingleObject(m_hNextSkeletonEvent, 0))
	{
		NUI_SKELETON_FRAME skeletonFrame = { 0 };

		// Get the skeleton frame that is ready
		if (SUCCEEDED(m_pNuiSensor->NuiSkeletonGetNextFrame(0, &skeletonFrame)))
		{
			// Process the skeleton frame
			SkeletonFrameReady(&skeletonFrame);
		}
	}
}