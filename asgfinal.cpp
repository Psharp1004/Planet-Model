#include "asgfinal.h"
#include "glheader.h"
#include "vector3f.h"
#include <ctype.h>

AsgFinal::AsgFinal(){
	Vector3f cam_origin(7.0f, 0.0f, 7.0f);
	camera = new AdvancedCamera(cam_origin, 0.0f, 0.0f);
	camera->setMouseSensitivity(0.1f);
	camera->restrainY = false;
	camera->rotateY(40.0f);
}

AsgFinal::~AsgFinal(){
	
}

/* Update function */
void AsgFinal::update(){ //called second
  
}

/* Draw function */
void AsgFinal::display(){ //called first
	gluLookAt(camera->position.x, camera->position.y, camera->position.z,
			  camera->lookat.x, camera->lookat.y, camera->lookat.z,
			  camera->up.x, camera->up.y, camera->up.z);
	planet.display();
}

/* Keyboard handling */
void AsgFinal::keyboard(unsigned char key, int x, int y){
	planet.keyboard(key, x, y);

	// I finally realised I could have
	// been using toupper since the beginning
	// of time. Duh.

	// Upper case letter comparison
	switch(toupper(key)){
		
		// Move forward
		case 'W':
		camera->move(camera->camSpeed);
		break;

		case 'S':
		camera->move(-camera->camSpeed);
		break;

		case 'D':
		camera->strafe(camera->camSpeed);
		break;

		case 'A':
		camera->strafe(-camera->camSpeed);
		break;
	}
}

/* Reshape func */
void AsgFinal::reshape(int width, int height){
	camera->reshapeFrustum(width, height, 0.05f, 100.0f, 65.0f);
}

/* Mouse passive fun */
void AsgFinal::mousePassive(int x, int y){
	camera->rotateWithMouse(x, y);
}
