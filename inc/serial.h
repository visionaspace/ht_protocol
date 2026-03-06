#include <stdint.h>
#include <termios.h>
#include "common_types.h"

#ifndef READSERIAL_H
#define READSERIAL_H

/* UART definitions here */
#define SERIAL_UART_NUM_BYTES_READ     1
#define SERIAL_INCLUDE_MARKERS         127
#define SERIAL_UART_SUCCESS            0

/* UART Error definitions */
#define SERIAL_UART_SUCCESS            0
#define SERIAL_UART_ERROR             -1
#define SERIAL_UART_BADDEVICE_ERR     -2
#define SERIAL_UART_OPEN_ERR          -3
#define SERIAL_UART_SETATTR_ERR       -4
#define SERIAL_MAX_READ_EXCEEDED      -5

int32 serialWrite(int32 fd, char *msg, int32 size);
int32 serialRead(int32 fd, char *buffer, char start_byte, char end_byte, uint8 include_markers, uint16 max_read_size);
int32 serialClose(int32 fd);
int32 serialOpen(const char *SerialPort);

#endif // READSERIAL_H
