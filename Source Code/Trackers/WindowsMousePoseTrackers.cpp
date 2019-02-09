/*********************************************************************************/
/*                                                                               */
/*                            WindowsMousePoseTrackers.cpp                       */
/*                                                                               */
/*********************************************************************************/

#include "stdafx.h"

#include "../Useful/Useful.h"
#include "../Useful/fMessageBox.h"
#include "../Useful/fOutputDebugString.h"

#include "../OpenGLObjects/OpenGLWindows.h"
#include "WindowsMousePoseTrackers.h"

#include <math.h>

using namespace PsyPhy;

WindowsMousePoseTracker::WindowsMousePoseTracker( OpenGLWindow *ptr, double gain ) {
	window = ptr;
	this->gain = gain;
	TimerStart( timer );
	PoseTracker();
}

// Define a common method to determine the timestamp for all MousePoseTrackers.
double WindowsMousePoseTracker::GetTime( void ) { 
	// Time is the elapsed time since this tracker instance was initialized.
	return( TimerElapsedTime( timer ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WindowsMouseRollPoseTracker::WindowsMouseRollPoseTracker( OpenGLWindow *window, double gain ) {
	this->window = window;
	this->gain = gain;
	this->lastX = window->mouseCumulativeX;
	this->lastY = window->mouseCumulativeY;
	CopyVector( eulerAngles, zeroVector );
	WindowsMousePoseTracker::WindowsMousePoseTracker( window, gain );
}

bool WindowsMouseRollPoseTracker::Update ( void ) {
	// Here we check the state of the key and move one gain step per cycle.
	// If framerate is constant, velocity should be constant.
	// An alternative approach would be to move according to the number of key down
	// events since the last cycle. That would allow for finer control and benefit from the
	// accelerating nature of events when you hold down a key.
	static double low_gain = 0.01;
	static double high_gain = 0.1;
	if ( window->Key[VK_SHIFT] && window->Key[VK_CONTROL] ) {
		if ( window->Key[VK_LEFT] ) eulerAngles[YAW]   += high_gain;
		if ( window->Key[VK_RIGHT] ) eulerAngles[YAW]  -= high_gain;
		if ( window->Key[VK_UP] ) eulerAngles[PITCH]   += high_gain;
		if ( window->Key[VK_DOWN] ) eulerAngles[PITCH] -= high_gain;
	}
	else if ( window->Key[VK_SHIFT] ) {
		if ( window->Key[VK_LEFT] ) eulerAngles[YAW]	+= low_gain;
		if ( window->Key[VK_RIGHT] ) eulerAngles[YAW]	-= low_gain;
		if ( window->Key[VK_UP] ) eulerAngles[PITCH]	+= low_gain;
		if ( window->Key[VK_DOWN] ) eulerAngles[PITCH]	-= low_gain;
	}
	return true;
}

bool WindowsMouseRollPoseTracker::GetCurrentPoseIntrinsic( TrackerPose &pose ) {

	// Place the position of this simulated tracker.
	CopyVector( pose.pose.position, zeroVector );

	// When using this tracker in simulation, it is useful to have the ability 
	// to raise and lower the hand.

	// Shift position up or down.
	if ( window->Key['W'] ) pose.pose.position[Y] = 250.0;
	if ( window->Key['X'] ) pose.pose.position[Y] = -250.0;

	// Shift position left or right.
	if ( window->Key['L'] ) pose.pose.position[X] =  100.0;
	if ( window->Key['K'] ) pose.pose.position[X] =	  50.0;
	if ( window->Key['J'] ) pose.pose.position[X] =  -50.0;
	if ( window->Key['H'] ) pose.pose.position[X] = -100.0;

	if ( window->Key[VK_SHIFT] ) {
		eulerAngles[PITCH] += ( window->mouseCumulativeY - lastY ) * gain;
		eulerAngles[YAW] += ( window->mouseCumulativeX - lastX ) * gain;
	}
	else eulerAngles[ROLL] = ( window->mouseCumulativeX - window->mouseCumulativeY ) * gain;
	lastX = window->mouseCumulativeX;
	lastY = window->mouseCumulativeY;

	// Compute the orientation output based on the mouse position and the results
	// of the various key presses handled in the Update() method.
	Quaternion rollQ, pitchQ, yawQ;
	Quaternion intermediateQ;
	// A feature of the Oculus window on the laptop screen is that you can move the mouse infinitely in any direction.
	// When the cursor gets to the edge of the screen, it wraps around to the other side.
	// Here we use this feature to allow the roll angle to move without a limit.
	SetQuaternion( rollQ, eulerAngles[ROLL], kVector );
	// Add in the pitch and yaw components, computed from eulerAngles[] that were modulated with key presses in Update().
	SetQuaternion( pitchQ, eulerAngles[PITCH], iVector );
	MultiplyQuaternions( intermediateQ, pitchQ, rollQ );
	SetQuaternion( yawQ, eulerAngles[YAW], jVector );
	MultiplyQuaternions( pose.pose.orientation, yawQ, intermediateQ );
	pose.time = GetTime();
	pose.visible = true;
	return pose.visible;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WindowsArrowPoseTracker::WindowsArrowPoseTracker( OpenGLWindow *window, double gain ) {
	this->window = window;
	this->gain = gain;
	CopyVector( eulerAngles, zeroVector );
	WindowsMousePoseTracker::WindowsMousePoseTracker( window, gain );
}

bool WindowsArrowPoseTracker::Update ( void ) {
	// Here we check the state of the key and move one gain step per cycle.
	// If framerate is constant, velocity should be constant.
	// An alternative approach would be to move according to the number of key down
	// events since the last cycle. That would allow for finer control and benefit from the
	// accelerating nature of events when you hold down a key.
	if ( !window->Key[VK_SHIFT] ) {
		if ( window->Key[VK_CONTROL] && window->Key[VK_LEFT] ) eulerAngles[YAW] -= gain;
		else if ( window->Key[VK_LEFT] ) eulerAngles[ROLL] -= gain;

		if ( window->Key[VK_CONTROL] && window->Key[VK_RIGHT] ) eulerAngles[YAW] += gain;
		else if ( window->Key[VK_RIGHT] ) eulerAngles[ROLL] += gain;

		if ( window->Key[VK_UP] ) eulerAngles[PITCH] -= gain;
		if ( window->Key[VK_DOWN] ) eulerAngles[PITCH] += gain;
	}
	return true;
}

bool WindowsArrowPoseTracker::GetCurrentPoseIntrinsic( TrackerPose &pose ) {

	// Place the position of this simulated tracker.
	CopyVector( pose.pose.position, zeroVector );

	// Compute the orientation output based on the mouse position and the results
	// of the various key presses handled in the Update() method.
	Quaternion rollQ, pitchQ, yawQ;
	Quaternion intermediateQ;
	// Chain roll, pitch and yaw components, computed from eulerAngles[] that were modulated with key presses in Update().
	SetQuaternion( rollQ, eulerAngles[ROLL], kVector );
	SetQuaternion( pitchQ, eulerAngles[PITCH], iVector );
	SetQuaternion( yawQ, eulerAngles[YAW], jVector );
	MultiplyQuaternions( intermediateQ, pitchQ, rollQ );
	MultiplyQuaternions( pose.pose.orientation, yawQ, intermediateQ );
	pose.time = GetTime();
	pose.visible = true;
	return pose.visible;
}
