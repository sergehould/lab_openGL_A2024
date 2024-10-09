#ifndef UTIL_H
#define	UTIL_H

long map(long x, long in_min, long in_max, long out_min, long out_max);
float mapf(float x, float in_min, float in_max, float out_min, float out_max);
void glPrintf(double x, double y, double z, const char* format, ...);
void open_gl_init(void);
#endif

