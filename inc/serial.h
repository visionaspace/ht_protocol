#include <stdint.h>
#include <termios.h>
#include "common_types.h"

#ifndef READSERIAL_H
#define READSERIAL_H

#define HT_SERIAL_BAUDRATE             115200
#define HT_SERIAL_TIMEOUT              60
#define HT_READ_BYTES                  1
#define ENABLE_PARITY                  1
#define DISABLE_PARITY                 0

/* UART definitions here */
#define ENABLE_PARITY                  1
#define DISABLE_PARITY                 0
#define SERIAL_UART_NUM_BYTES_READ     1
#define SERIAL_INCLUDE_MARKETS         127
#define SERIAL_UART_OPEN_FLAGS         O_RDWR
#define SERIAL_UART_SUCCESS            0

/* UART Error definitions */
#define SERIAL_UART_SUCCESS            0
#define SERIAL_UART_ERROR             -1
#define SERIAL_UART_BADDEVICE_ERR     -2
#define SERIAL_UART_OPEN_ERR          -3
#define SERIAL_UART_SETATTR_ERR       -4
#define SERIAL_MAX_READ_EXCEEDED      -5

speed_t getBaudrateMacro(uint32 baudrate);
int32 serialInit(const char *serial_port, int32 baudRate, int32 timeout, int32 minBytes, uint8 parity);
int32 serialWrite(int32 fd, char *msg, int32 size);
int32 serialRead(int32 fd, char *buffer, char start_byte, char end_byte, uint8 include_markers, uint16 max_read_size);
int32 serialClose(int32 fd);
int32 serialOpen(const char *SerialPort);

#endif // READSERIAL_H
