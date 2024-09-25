#ifndef SERIAL_H
#define	SERIAL_H
#include <stdint.h>

#define PORT    COM5
//#define	get_byte	rec_one_int8_nb
int test(void);
int serial_init(void);
int serial_init2(void);
char serial_read(void);
void serial_close(void);
int16_t rec_one_int16_b();
void put_int16(int16_t data);
int16_t get_int16();
char get_byte();
void put_byte(char tx);


#endif

