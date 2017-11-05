#include "asgfinal.h"
#include "glheader.h"
#include <stdlib.h>
#include <stdio.h>

#define MS_UPDATE 33
using namespace::std;

AsgFinal *asgfinal;

void reshape (int width, int height) {  
	asgfinal->reshape(width, height);
	printf("CALLED");
}  

void mousePassive(int x, int y){
	asgfinal->mousePassive(x, y);
}

void keyboard(unsigned char c, int x, int y){
	asgfinal->keyboard(c, x, y);
}

void update(int value){
	asgfinal->update();
	glutTimerFunc(MS_UPDATE, update, value);
}

void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	asgfinal->display();

	glutSwapBuffers();
	glutPostRedisplay();
}

int init(){
	// Instantiate asgfinal
	asgfinal = new AsgFinal();

	// OpenGL standard configurations
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);

	// Enables
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

	// Projection mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 1.3f, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Which face should be culled
	glCullFace(GL_BACK);

	// Light configurations
	float l_position[] = {5.0f, 5.0f, 5.0f, 1.0f};
	float l_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
	float l_diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
	float l_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, l_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l_specular);

	// General material configurations
	float m_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
	float m_specular[] = {0.1f, 0.1f, 0.1f, 1.0f};
	float m_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	float shiny_factor = 10.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny_factor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
}

int main(int argc, char** argv){

	// Initialisation functions
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Final Project - Planet");

	// Init function
	init();

	// Callback functions setup
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mousePassive);
	glutTimerFunc(MS_UPDATE, update, 1);

	// Make camera look to the origin
	/*gluLookAt (5.0f, 5.0f, 10.0f,
			   0.0f, 0.0f, 0.0f,
			   0.0f, 1.0f, 0.0f);*/

	// Starts main loop
	glutMainLoop();

	// Everything went better than expected
	return 0;

}
