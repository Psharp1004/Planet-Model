#include "advcam.h"
#include "glheader.h"
#include <stdio.h>
#define PI 3.1415

/* Constructor initialises default values for 
all camera attributes */
AdvancedCamera::AdvancedCamera(Vector3f origin, float xrot, float yrot){
	position = origin;
	forward = Vector3f(0.0f, 0.0f, -1.0f);
	right = Vector3f(1.0f, 0.0f, 0.0f);
	up = Vector3f(0.0f, 1.0f, 0.0f);
	rotation = Vector3f(0.0f, 0.0f, 0.0f);
	lookat = position + forward;
	maxYAngle = 60.0f;
	camSpeed = 0.5f;
	restrainY = true;

	rotateX(xrot);
	rotateY(yrot);
}

AdvancedCamera::~AdvancedCamera(){}

/* Method to rotate around the Y axis,
effectively moving the horizontal lookat */
void AdvancedCamera::rotateY(float angle){

	// Clamp rotation
	rotation.y += angle;
	if (rotation.y > 360.0f) rotation.y -= 360.0f;
	if (rotation.y < 0.0f) rotation.y = -rotation.y;

	// Moving in the unitary circle
	float cosine = cos ( angle * (PI/180.0f));
	float sine = sin( angle * ( PI/180.0f));

	// Forward is the cross product of the
	// applied sine and cosine calculations
	forward = (forward * cosine) - (right * sine);
	forward = forward.normalized();

	// Find new right vector
	right = forward * up;

	// Update lookat vector
	updateLook();
}

/* Method to rotate around the X axis,
effectively moving the vertical lookat */
void AdvancedCamera::rotateX(float angle){
	
	// Clamp rotation and limit range
	// --recently learned  that his is a way to get rid of
	// gimbal's lock since open gl uses euler =DD
	// DO NOT REMOVE FOR GODS SAKE PEOPLE THANKS
	// this is what caused me a problem I tried to
	// solve for almost an hour
	rotation.x += angle;

	if (restrainY){
		if (rotation.x > maxYAngle){
			rotation.x = maxYAngle;
			return;
		} else if (rotation.x < -maxYAngle){
			rotation.x = -maxYAngle;
			return;
		}
	} else {
		if (rotation.x > 360.0f) rotation.x -= 360.0f;
		if (rotation.x < 0.0f) rotation.x = -rotation.x;
	}

	// Unitary circle calculation
	float cosine = cos ( angle * (PI/180.0f));
	float sine = sin( angle * ( PI/180.0f));

	// Forward is the cross product of the
	// applied sine and cosine calculations
	forward = (forward * cosine) - (up * sine);
	forward = forward.normalized();

	// Update lookat vector
	updateLook();
}

/* Move camera forward or backwards */
void AdvancedCamera::move(float speed){
	position = position + (forward * speed);
	updateLook();
}

/* Strafe camera to the left or to the right */
void AdvancedCamera::strafe(float speed){
	position = position + (right * speed);
	updateLook();
}

/* Update lookat vector */
void AdvancedCamera::updateLook(){
	lookat = position + forward;
}

/* Update rotation based upon mouse movement */
void AdvancedCamera::rotateWithMouse(int xmouse, int ymouse){
	currentCursorMode = glutGet(GLUT_WINDOW_CURSOR);
	glutSetCursor(GLUT_CURSOR_NONE);

	int cX = viewport_width / 2;
	int cY = (viewport_height / 2);

    int dX = cX - xmouse; //merry xmouse guys ( ...)
    int dY = ymouse - cY; 

    if(dX != 0 || dY != 0) {
        rotateX(dY * mouseSensitivity);
        rotateY(dX * mouseSensitivity);
        glutWarpPointer(cX, cY);

        // Without this camera won't update
        // -- use if need to restrain rotation
        updateLook();
    }

	//glutSetCursor(currentCursorMode);
}

/* Set mouse sensitivity when needed (and when one wants)
to use the mouse to rotate the camera. 1.0f = full Delta speed*/
void AdvancedCamera::setMouseSensitivity(float sensitivity){
	mouseSensitivity = sensitivity;
}

/* Reshape camera frustum */
void AdvancedCamera::reshapeFrustum(int width, int height, float cam_near, float cam_far, float angle){

	// Set attributes
	viewport_width = width;
	viewport_height = height;
	near_plane = cam_near;
	far_plane = cam_far;
	view_angle = angle;

	// Re-configure projection matrix and viewport
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective((GLfloat)view_angle, (GLfloat)viewport_width / (GLfloat)viewport_height, near_plane, far_plane); 
	glMatrixMode(GL_MODELVIEW);

}