/*******************************************************************************
	Author		Date		Version			Description
	
	Note: to set platform, properties->General-> platform Toolset -> select Visual Studio 2019 (v142)

*******************************************************************************/

#include "freeglut/include/GL/glut.h"
#include "freeglut/include/GL/freeglut.h"
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include "serial.h"
#include "tick.h"
#include "util.h"
#include "opengl_init.h"
#include "configuration.h"


// Callback function for rendering a scene
void render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -16);
	x_y_z_draw(8, 0.2);
	glPrintf(0, -4, 0, "It %s","works!");
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	glutSolidSphere(1.0f, 60, 60); // 
	glutSwapBuffers();
}



int main(int argc, char **argv) {
	// initializes the serial port
	int status = 0;
	#ifndef		SIMULATION
		status = serial_init();
	#endif
	tick_init();
	/* Initializes openGL specifying screen size, screen position, render 
		callback, normal keys callback and special keys callback  */
	open_gl_init(640, 640, 10, 10,render,  argc, argv);
}


