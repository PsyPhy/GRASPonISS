#pragma once 

#include "../OculusInterface/OculusInterface.h"
#include "../OpenGLObjects/OpenGLObjects.h"
#include "../OpenGLObjects/OpenGLViewpoints.h"

// Allows one to use the PsyPhy OpenGL object libraries with the Oculus.
// Using this viewpoint, instead of the standard OpenGLObjects viewpoint sets up the GL transformations
//  to be compatible with the OculusMapper windows and buffers.

namespace PsyPhy {

class OculusViewpoint : public Viewpoint {

private:

	OculusMapper *oculusMapper;
	double upTan;
	double downTan;
	double leftTan;
	double rightTan;

public:

	// Sets up the viewpoint parameters compatible with the Oculus viewing frustrum.
	OculusViewpoint( OculusMapper *oculusMapper = nullptr, double ipd = 0.06, double nearest = 0.01, double farthest = 1000.0 );
	void Apply( OpenGLWindow *window, Eye eye );
	void GetEyeProjections ( int eye, OVR::Matrix4f *view, OVR::Matrix4f *projection );

};

};