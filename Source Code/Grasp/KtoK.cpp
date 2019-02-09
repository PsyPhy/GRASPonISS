// Grasp.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "../OculusInterface/OculusInterface.h"

#include "../Useful/Timers.h"
#include "../DexServices/DexServices.h"
#include "../GraspVR/GraspVR.h"
#include "GraspTaskManagers.h"

using namespace Grasp;
using namespace PsyPhy;

/// KtoK

void KtoK::EnterPresentTarget( void ) {
	// Show the subject where to point.
	//renderer->positionOnlyTarget->Enable();
	//renderer->positionOnlyTarget->SetColor( Translucid( CYAN ) );
	//renderer->aimingErrorSphere->Disable();
	// Visualize the hand, but not its actual roll orientation, by using the kkTool.
	// The orientation will be reflected by the color.
	renderer->kkTool->Enable();
	StartHandAiming();
	// The target orientation for the hand.
	SetDesiredHandRoll( trialParameters[currentTrial].targetOrientation, hapticTargetOrientationTolerance );
	// Set a timeout to achieve the target orientation of the hand.
	TimerSet( alignHandTimer, alignHandTimeout );
	// Do all the default actions as well.
	GraspTaskManager::EnterPresentTarget();
}

void KtoK::EnterObtainResponse( void ) {
	// Show the visual representation of the hand.
	renderer->kTool->Enable();
	StartHandAiming();
	// Do all the default actions as well.
	GraspTaskManager::EnterObtainResponse();
}

