#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "serial.h"
#include "common_types.h"

/* This function will read a message that starts with start_byte and ends with end_byte */
int32 serialRead(int32 fd, char *buffer, char start_byte, char end_byte, uint8 include_markers, uint16 max_read_size) {

    int32 readSize = 0, index = 0;
    int32 Status = -1;
    boolean hasMessage = false;
    char aux;

    while (Status != SERIAL_UART_SUCCESS) {

        /* NOTE: Will not return until at least 1 byte is received
         * if configured with minBytes > 0 and timeout > 0 */
        readSize = read(fd, &aux, SERIAL_UART_NUM_BYTES_READ);

        /* Ensure that don't occur buffer overflow */
        if(index >= max_read_size) {
            return SERIAL_MAX_READ_EXCEEDED;
        }

        /* End of Message or timeout */
        if (hasMessage && (readSize <= 0 || aux == end_byte) ) {
            Status = SERIAL_UART_SUCCESS;

            /* This will include the end byte in the message */
            if (include_markers == SERIAL_INCLUDE_MARKETS) {
                buffer[index++] = aux;

                /* Check if it is possible to add one more byte to end the message */
                if(index >= max_read_size) {
                    return SERIAL_MAX_READ_EXCEEDED;
                }
            }
            buffer[index++] = '\0';
            hasMessage = false;
            break;
        } else if(aux == start_byte) {
            if(hasMessage) {
                /* Lost message return error */
                return SERIAL_UART_ERROR;
            } else {
                /* Start new message here normally */
                hasMessage = true;
                /* This will include the start byte in the message */
                if (include_markers == SERIAL_INCLUDE_MARKETS) {
                    buffer[index++] = aux;
                }
            }
        } else if (hasMessage) {
            /* Receiving the content of the message */
            buffer[index++] = aux;

        } else {
            /* TODO: Something very wrong happened here.
               Never should reach this condition */
            return SERIAL_UART_ERROR;
        }
    }

    return index;
}

int32 serialClose(int32 fd) {
    return (int32)close(fd);
}

/* This function will write to a given serial interface */
int32 serialWrite(int32 fd, char *msg, int32 size) {

    int32 sizeOut = 0;

    sizeOut = write(fd, (char *) msg, size);

    if (sizeOut == -1) {
        if (errno == EBADF) {
            return -2;
        } else {
            return -3;
        }
    }

    return sizeOut;
}