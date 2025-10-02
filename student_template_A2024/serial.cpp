#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <conio.h>
#include "configuration.h"

//using namespace std;
#pragma warning(disable : 4996)
// Choose the serial port name. 
// COM ports higher than COM9 need the \\.\ prefix, which is written as
// "\\\\.\\" in C because we need to escape the backslashes.
char buffer[100], buffer2[100];
unsigned char readBuf[1];
int rx1, rx2, _range, center, status, i = 0, j = 0, k = 0, m = 0, bar;
DWORD nb_bytes;
struct timeval start;	/* starting time */
struct timeval end, boot_time;	/* booting time and ending time */
unsigned long e_usec, boot_sec, end_sec, current_ms;	/* elapsed microseconds */
//166x300

// Choose the baud rate (bits per second).  
uint32_t baud_rate = 115200;

static void print_error(const char* context);
static int write_port(HANDLE port, uint8_t* buffer, size_t size);
HANDLE port;

int serial_init(void) {
    std::string portName;
    std::cout << "Enter the Com Port. (E.g. 5).\nTo find out the Com Port, open Device Manager and then look at the Ports tab.\n";
    std::cin >> portName;

    std::string fullPortName = "\\\\.\\COM" + portName;
    port = CreateFile(fullPortName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    //wchar_t pszPortName[] = L"5"; //com port id
    //wchar_t PortNo[20] = { 0 }; //contain friendly name
    //printf_s("Enter the Com Port. (E.g. 5).\nTo find out the Com Port, open Device Manager and then look at the Ports tab.\n");
    //wscanf_s(L"%s", pszPortName, (unsigned)_countof(pszPortName));
    //swprintf_s(PortNo, 20, L"\\\\.\\COM%s", pszPortName);
    //sprintf(PortNo,"\\\\.\\COM%d", port_num);
    //printf("fffff");
    //system("pause");

    //port = CreateFile(PortNo, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    //port = CreateFile(L"\\\\.\\COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (port == INVALID_HANDLE_VALUE)
    {
        //print_error(device);
        printf_s("\n Port can't be opened\n\n");
        //system("pause");
        return -1;
    }
    else printf_s("\nPort opened successfully\n\n");
    // Flush away any bytes previously read or written.
    BOOL success = FlushFileBuffers(port);
    if (!success)
    {
        // print_error("Failed to flush serial port");
        CloseHandle(port);
        return -1;
    }

    // Configure read and write operations to time out after 100 ms.
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 0;
    //timeouts.ReadTotalTimeoutConstant = 100; 
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    //timeouts.WriteTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    success = SetCommTimeouts(port, &timeouts);
    if (!success)
    {
        //print_error("Failed to set serial timeouts");
        CloseHandle(port);
        return -1;
    }

    // Set the baud rate and other options.
    DCB state = { 0 };
    state.DCBlength = sizeof(DCB);
    state.BaudRate = baud_rate;
    state.ByteSize = 8;
    state.Parity = NOPARITY;
    state.StopBits = ONESTOPBIT;
    success = SetCommState(port, &state);
    if (!success)
    {
        //print_error("Failed to set serial settings");
        CloseHandle(port);
        return -1;
    }
    if (port == INVALID_HANDLE_VALUE) { return -1; }
    return 0;
}

/* non-blocking. Returns last value if no recption*/
char serial_read(void)
{
        static unsigned char last = 0;
        int ch = 0;
        fflush(stdin);
        //if (kbhit()) {//Check whether there is data in the buffer
        //    ch = getch();//Read the data in the buffer in the form of characters
        //}
        status = ReadFile(port, readBuf, 1, &nb_bytes, NULL); // non-blocking
        //if (nb_bytes == 0) return 0xff; //empty
        //PurgeComm(port, PURGE_RXCLEAR);
        if (nb_bytes == 0)
        {
            return last; //empty
        }
        else { 
            last = readBuf[0];
            return last;
        }
}

void serial_close(void) {
    CloseHandle(port);
}
#ifndef SIMULATION
/* Function that sends one byte to the serial port */
void put_byte(char tx) {
    uint8_t buff[1];
    static int cnt = 0;
    buff[0] = tx;
    //if (cnt++ == 10) {
        write_port(port, buff, 1);
        cnt = 0;
    //}
}
#endif

// Writes bytes to the serial port, returning 0 on success and -1 on failure.
static int write_port(HANDLE port, uint8_t* buffer, size_t size)
{
    DWORD written;
    BOOL success = WriteFile(port, buffer, size, &written, NULL);
    if (!success)
    {
        print_error("Failed to write to port");
        return -1;
    }
    if (written != size)
    {
        print_error("Failed to write all bytes to port");
        return -1;
    }
    return 0;
}


static void print_error(const char* context)
{
    DWORD error_code = GetLastError();
    char buffer[256];
    DWORD size = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
        NULL, error_code, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        buffer, sizeof(buffer), NULL);
    if (size == 0) { buffer[0] = 0; }
    fprintf(stderr, "%s: %s\n", context, buffer);
}


/*
 Blocking polling function that receives two bytes 
 from the serial port and merge them as an int16 
 using a start byte, a stop byte and checksum byte.
 Returns the received character if no error.
 Returns -1 if there is an error.
*/

#define START_BYTE 0x7E  // 126
#define STOP_BYTE 0x7F  //127
int16_t rec_one_int16_b() {
    uint8_t byte;
    uint8_t start_byte = START_BYTE;
    uint8_t stop_byte = STOP_BYTE;
    uint8_t data_bytes[2];
    uint8_t checksum = start_byte + stop_byte;

    while ((byte = serial_read()) != start_byte) {
        // Wait for the start byte
    }

    for (int i = 0; i < 2; i++) {
        data_bytes[i] = serial_read();
        checksum += data_bytes[i];
    }
    byte= serial_read();
    if (byte != checksum) {
        // Checksum error
        return -1;
    }

    if (serial_read() != stop_byte) {
        // Stop byte error
        return -1;
    }
    return (data_bytes[0] << 8) | data_bytes[1];
}

int16_t get_int16(int p) {
    uint8_t byte;
    uint8_t start_byte = START_BYTE;
    uint8_t stop_byte = STOP_BYTE;
    uint8_t data_bytes[2];
    uint8_t checksum = start_byte + stop_byte;
    static int16_t last = 0;

    if ((byte = serial_read()) != start_byte) {
        return last;
    }

    for (int i = 0; i < 2; i++) {
        data_bytes[i] = serial_read();
        checksum += data_bytes[i];
    }
    byte = serial_read();
    if (byte != checksum) {
        // Checksum error
        return last;
    }

    if (serial_read() != stop_byte) {
        // Stop byte error
        return last;
    }
    last = (data_bytes[0] << 8) | data_bytes[1];
    if(p== PURGE) PurgeComm(port, PURGE_RXCLEAR); // clears rx buffer 
    return last;
}
#ifndef SIMULATION
char get_byte(int p) {
    //return serial_read();
    char rx= serial_read();
    if(p == PURGE) PurgeComm(port, PURGE_RXCLEAR); // clears rx buffer 
    return rx;
}
#endif
void put_int16(int16_t data) {
    uint8_t buff[1];
    uint8_t start_byte = START_BYTE;
    uint8_t stop_byte = STOP_BYTE;
    uint8_t data_bytes[2] = { (data >> 8) & 0xFF, data & 0xFF };
    uint8_t checksum = start_byte + stop_byte + data_bytes[0] + data_bytes[1];

    buff[0] = start_byte;
    put_byte(start_byte);
    //write_port(port, buff, 1);
    buff[0] = data_bytes[0];
    put_byte(data & 0xFF);
    //write_port(port, buff, 1);
    buff[0] = data_bytes[1];
    put_byte((data >> 8) & 0xFF);
    //write_port(port, buff, 1);
    buff[0] = checksum;
    put_byte(checksum);
    //write_port(port, buff, 1);
    buff[0] = stop_byte;
    put_byte(stop_byte);
    //write_port(port, buff, 1);




    //UART2_Write(start_byte);
    //UART2_Write(data_bytes[0]);
    //UART2_Write(data_bytes[1]);
    //UART2_Write(checksum);
    //UART2_Write(stop_byte);
}
