#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#ifdef	OPENGL_PROJECT
#include "freeglut/include/GL/glut.h"
#include "freeglut/include/GL/freeglut.h"
#endif

/* Map function */

/* Long version */
long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/* Float version */
float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#ifdef	OPENGL_PROJECT
/* Prints at position x,y,z */
void glPrintf(double x, double y, double z, const char* format, ...) {
	char buf[512];
	va_list vl;
	va_start(vl, format);
	vsnprintf(buf, 511, format, vl);
	va_end(vl);
	glRasterPos3f(x, y, z);
	for (int i = 0; buf[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, buf[i]);
	}

}


#endif
