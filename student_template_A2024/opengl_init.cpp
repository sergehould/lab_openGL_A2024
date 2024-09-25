#include "freeglut/include/GL/glut.h"
#include "freeglut/include/GL/freeglut.h"
#include "opengl_init.h"

void changeSize(int w, int h)
{
	float ratio = w * 1.0 / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(90, ratio, 1, 100);
	glMatrixMode(GL_MODELVIEW);
}

// Application-specific initialization: Set up global lighting parameters
// and create display lists.
void init2() {
	glClearColor(1, 1, 1, 1.0); // white background
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void open_gl_init(int w, int h, int x, int y, void (*callback1)() , void (*callback2)(unsigned char a, int b, int c), void (*callback3)(int a, int b, int c), int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(x, y);
	glutInitWindowSize(w, h);
	glutCreateWindow("Welcome to Vanier CET department");
	glEnable(GL_DEPTH_TEST); // to avoid back get drawn to the display over the top of the model at the front
	glutReshapeFunc(changeSize);
	glutIdleFunc(callback1);
	glutDisplayFunc(callback1);
	glutKeyboardFunc(callback2);
	glutSpecialFunc(callback3);
	init2();
	glutMainLoop();

}

/* Draws the z-y-z axis */
void x_y_z_draw(double l, double d) {
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, black);
	glLineWidth(1);
	//Y axis
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, l, 0);
	glEnd();
	glPushMatrix();
	glTranslatef(0, l, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(d, 2 * d, 15, 15);
	glPopMatrix();
	//  letter
	glRasterPos3f(-l * 0.1, l + l * 0.2, 0);// position
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'Y');

	//Z axis
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, l);
	glEnd();
	glPushMatrix();
	glTranslatef(0, 0, l);
	glutSolidCone(d, 2 * d, 15, 15);
	glPopMatrix();
	// Z letter
	glRasterPos3f(-l * 0.1, 0, l + l * 0.2);// position
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'Z');

	//X axis
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(l, 0, 0);
	glEnd();
	glPushMatrix();
	glTranslatef(l, 0, 0);
	glRotatef(90, 0, 1, 0);
	glutSolidCone(d, 2 * d, 15, 15);
	glPopMatrix();

	//  letter
	glRasterPos3f(l + l * 0.2, 0, l * 0.1);// position
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'X');

}