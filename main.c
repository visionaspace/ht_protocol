#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>

#include "common_types.h"
#include "serial.h"
#include "ht_protocol.h"
#include "vlog.h"

pthread_mutex_t IOMutex;

#define WRITE_TASK_NOTIFY(_format, ...) \
{                                       \
    pthread_mutex_lock(&IOMutex);       \
    VLOG_NOTIFY(_format, ##__VA_ARGS__);\
    pthread_mutex_unlock(&IOMutex);     \
}                                       \

#define READ_TASK_NOTIFY(_format, ...)  \
{                                       \
    pthread_mutex_lock(&IOMutex);       \
    VLOG_CMD(_format, ##__VA_ARGS__);   \
    pthread_mutex_unlock(&IOMutex);     \
}                                       \

#define MAX_FOR(type) (1 << sizeof(type))

#define SAFE_INCR(var) var = (var == MAX_FOR(var)) ? 1 : var + 1

#define SEND_MSG(Payload, Scope) \
{                                                                                       \
    char EncodedMessage[HT_PROTOCOL_CALC_ENCODED_SIZE(sizeof(Payload0))];               \
    {Scope};                                                                            \
    HT_PROTOCOL_InitMsg(&Message, 1, 2, 3, sizeof(Payload0), (void *) &Payload0, true); \
    HT_PROTOCOL_EncodeMsg(EncodedMessage, &Message);                                    \
    serialWrite(Fd, EncodedMessage, HT_PROTOCOL_CALC_ENCODED_SIZE(sizeof(Payload0)));   \
    WRITE_TASK_NOTIFY("Wrote message = %s", EncodedMessage);                            \
}                                                                                       \

const char *SerialPort = "/dev/pts/3";

void ReadTask(void) {

    int32 bytes_read = 0;
    static char Buffer[HT_MAX_MESSAGE_BYTES];

    int32 Fd = serialOpen(SerialPort);

    while(1) {
        bytes_read = serialRead(Fd, Buffer, HT_HW_START_CHAR, HT_HW_END_CHAR, 0, HT_MAX_MESSAGE_BYTES);

        if (bytes_read <= 0) {
            // printf("Error reading serial\n");
        } else {
            READ_TASK_NOTIFY("Received message = %s", Buffer);
        }
        READ_TASK_NOTIFY("Read Task running\n");
        usleep(500000);
    }
}

void WriteTask(void) {

    typedef struct {
        uint8  Info;
        uint8  Data;
        uint16 Temp;
        float  Pressure;
    } PayloadExample0_t;

    typedef struct {
        double Measure;
    } PayloadExample1_t;

    typedef struct {
        uint8 ControlBits;
    } PayloadExample2_t;

    PayloadExample0_t Payload0 = {.Info = 0x01};
    PayloadExample1_t Payload1 = {.Measure = 0x05};
    PayloadExample2_t Payload2 = {.ControlBits = 0x06};

    HT_PROTOCOL_Msg_t Message;

    int32 Fd = serialOpen(SerialPort);

    while(1) {
        int32 PayloadNum = rand() % 3;

        switch(PayloadNum) {
            case 0: {
                SEND_MSG(Payload0, {
                    SAFE_INCR(Payload0.Info);
                    SAFE_INCR(Payload0.Data);
                    SAFE_INCR(Payload0.Temp);
                    Payload0.Pressure += 0.1;
                });
                break;
            }
            case 1: {
                SEND_MSG(Payload1, {
                    Payload1.Measure += 0.1;
                });
                break;
            }
            case 2: {
                SEND_MSG(Payload2, {
                    SAFE_INCR(Payload2.ControlBits);
                });
                break;
            }
            default:
                break;
        }

        usleep(500000);
    }
}


int main(void) {

    pthread_mutex_init(&IOMutex, NULL);

    pthread_t ReadTaskThread, WriteTaskThread;

    if (pthread_create(&ReadTaskThread, NULL, (void *)&ReadTask, NULL) != 0) {
        printf("Error creating read task\n");
        return -1;
    }

    if (pthread_create(&WriteTaskThread, NULL, (void *)&WriteTask, NULL) != 0) {
        printf("Error creating write task\n");
        return -1;
    }

    pthread_join(ReadTaskThread, NULL);
    pthread_join(WriteTaskThread, NULL);

    while(1) {
        usleep(100000000);
    }

    return 0;
}
