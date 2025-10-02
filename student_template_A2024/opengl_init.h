#pragma once
/* Color sets */
static GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat yellow[] = { 1.0, 1.0, 0.0, 1.0 };
static GLfloat cyan[] = { 0.0, 1.0, 1.0, 1.0 };
static GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };
static GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
static GLfloat grey[] = { 0.752941, 0.752941, 0.752941, 1.0 };
//static GLfloat orange[] = { 0.89, 0.47, 0.20, 1.0 };
static GLfloat orange[] = { 1.0, 0.50, 0.0, 1.0 };
void changeSize(int w, int h);
void open_gl_init(int w, int h, int x, int y, void (*callback1)(), int argc, char** argv);
void x_y_z_draw(double l, double d);
