#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdint.h>

static LARGE_INTEGER li;
static long Freq = 0;// curTime = 0;

/* Initializes the tick lib  */
void tick_init(void){
	long temp;
	QueryPerformanceFrequency(&li);
	Freq = li.QuadPart;
	temp = Freq;
	//QueryPerformanceCounter(&li);
	//curTime = li.QuadPart / (Freq / 1000);// convert to sec
}

/*  Returns current tick in mS */
uint32_t tick_get(void) {
	uint32_t cur_time;
	/* asses time */
	QueryPerformanceCounter(&li);
	cur_time = li.QuadPart / (Freq / 1000); 
	return cur_time;
}
/*  Returns the difference between the current tick and the stamp argument in mS */
/*  Stress tested correctly with signed long long */			
uint32_t tick_diff(uint32_t stamp) {
	uint32_t diff;
	diff = tick_get() - stamp; // will always calculate correctly even if tick_get < stamp because uint32_t will remove the negative sign.
	return (uint32_t) diff;
}