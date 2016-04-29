/*********************************************************************************/
/*                                                                               */
/*                                OpenGLUseful.c                                 */
/*                                                                               */
/*********************************************************************************/

/* Some useful OpenGL routines. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <windows.h>
#include <mmsystem.h>

#include <gl/gl.h>
#include <gl/glu.h>

#include "OpenGLUseful.h"


/*********************************************************************************/

void glUsefulDefaultSpecularLighting( void ) {

  // Light definition
  GLfloat LightPosition[] = { 5000.0, 5000.0, 5000.0, 0.0 };
  GLfloat LightAmbient[]  = { 0.05, 0.05, 0.05, 1.0};
  GLfloat LightDiffuse[]  = { 0.050, 0.050, 0.050, 1.0};
  GLfloat LightSpecular[] = { 0.90, 0.90, 0.90, 1.0};

  glLightfv( GL_LIGHT0, GL_POSITION, LightPosition );
  glLightfv( GL_LIGHT0, GL_AMBIENT, LightAmbient );
  glLightfv( GL_LIGHT0, GL_DIFFUSE, LightDiffuse );
  glLightfv( GL_LIGHT0, GL_SPECULAR, LightSpecular );

  glEnable(GL_LIGHT0);

  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 
  glEnable( GL_BLEND );

  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glClearColor( 1.0F, 1.0F, 1.0F, 0.0F );



}

void glUsefulCodaLighting( void ) {

  // Light definition
  GLfloat LightPosition[] = {  5000.0,  5000.0, 5000.0, 0.0 };
  GLfloat LightAmbient[]  = { 0.25, 0.25, 0.25, 1.0};
  GLfloat LightDiffuse[]  = { 0.50, 0.50, 0.50, 1.0};
  GLfloat LightSpecular[] = { 0.50, 0.50, 0.50, 1.0};

  glLightfv( GL_LIGHT0, GL_POSITION, LightPosition );
  glLightfv( GL_LIGHT0, GL_AMBIENT, LightAmbient );
  glLightfv( GL_LIGHT0, GL_DIFFUSE, LightDiffuse );
  glLightfv( GL_LIGHT0, GL_SPECULAR, LightSpecular );

  glEnable(GL_LIGHT0);

  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 
  glEnable( GL_BLEND );

  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glClearColor( 1.0F, 1.0F, 1.0F, 0.0F );



}

void glUsefulAutoLighting( double intensity ) {

  float intensityf = (GLfloat) intensity;

  // Light definition
  GLfloat LightAmbient[]  = { intensityf, intensityf, intensityf, 1.0};
  GLfloat LightDiffuse[]  = { intensityf, intensityf, intensityf, 1.0};
  GLfloat LightSpecular[] = { 0.00, 0.00, 0.00, 1.0};

  glLightfv( GL_LIGHT0, GL_AMBIENT, LightAmbient );
  glLightfv( GL_LIGHT0, GL_DIFFUSE, LightDiffuse );
  glLightfv( GL_LIGHT0, GL_SPECULAR, LightSpecular );

  glEnable( GL_LIGHT0 );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 
  glEnable( GL_BLEND );

  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glClearColor( 1.0F, 1.0F, 1.0F, 0.0F );



}

/*********************************************************************************/

void glUsefulDefaultMaterial( void ) {

  // Material definition
  GLfloat MaterialAmbient[]   = { 0.25, 0.25, 0.25, 1.0};
  GLfloat MaterialDiffuse[]   = { 0.50, 0.50, 0.50, 1.0};
  GLfloat MaterialSpecular[]  = { 0.50, 0.50, 0.50, 1.0};
  GLfloat MaterialShininess[] = { 100.0 };

  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient );
  glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse );
  glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular );
  glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess );

  glEnable( GL_COLOR_MATERIAL );
  glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

}

///////////////////////////////////////////////////////////////////////////////////////////////////////

// Routines to initialize the GL graphics engine at the beginning of the program 
//  and prior to the rendering of each scene. 
void glUsefulInitializeDefault ( void ) {

	glUsefulAutoLighting( 0.7 );
	glUsefulDefaultMaterial();

	// Shade model
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glDisable( GL_CULL_FACE );							// glu drawing routines do not like backface culling.

}

// Get ready to do 3D rendering with GL.
void glUsefulPrepareRendering ( void ) {

	// This should be intergrated more cleanly into the OpenGLObjects system.
	// Perhaps it should be a method of OpenGLWindow.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}