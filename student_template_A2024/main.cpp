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


// Callback function for rendering a scene
void render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -2);
	x_y_z_draw(1, 0.02);
	glPrintf(0, -1, 0, "Good %s","day");
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	glutSolidSphere(0.1f, 60, 60); // 
	glutSwapBuffers();
}

// Callback function normal keyboard keys
void n_keys(unsigned char key,int x, int y)
{ 

}

// Callback function special keyboard keys
void s_keys(int key, int x, int y) {

}

int main(int argc, char **argv) {
	serial_init(); // initializes the serial port
	tick_init();
	/* Initializes openGL specifying screen size, screen position, render 
		callback, normal keys callback and special keys callback  */
	open_gl_init(640, 640, 10, 10,render, n_keys, s_keys,  argc, argv);
}


