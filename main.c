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

#define NOTIFY(_format, ...) \
{                                       \
    pthread_mutex_lock(&IOMutex);       \
    VLOG_NOTIFY(_format, ##__VA_ARGS__);\
    pthread_mutex_unlock(&IOMutex);     \
}                                       \

const char *SerialPort = "/dev/pts/6";

void ReadTask(void *Arg) {

    int32 bytes_read = 0;
    static char Buffer[HT_MAX_MESSAGE_BYTES];

    printf("Starting read_serial task\n");

    while(1) {

        // Removed globalData here
        //bytes_read = serialRead(globalData.serial_fd, str_buffer, HT_HW_START_CHAR, HT_HW_END_CHAR, 0, HT_MAX_MESSAGE_BYTES);
        /* bytes_read = serialRead(0, Buffer, HT_HW_START_CHAR, HT_HW_END_CHAR, 0, HT_MAX_MESSAGE_BYTES);

        if (bytes_read <= 0) {
            printf("Error reading serial\n");
        } else { */
            /* Message read. Process it */
            //process_read_msg(str_buffer, (uint16) bytes_read);
        // }
        // NOTIFY("Read Task running\n");
        // VLOG_NOTIFY("Read Task running\n");
        usleep(1000000);
    }
}

void WriteTask(void *Args) {

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

    PayloadExample0_t Payload0 = {.Info = 0x01, .Data = 0x02, .Temp = 0x03, .Pressure = 0x04};
    PayloadExample1_t Payload1 = {.Measure = 0x05};
    PayloadExample2_t Payload2 = {.ControlBits = 0x06};

    HT_PROTOCOL_Msg_t Message;

    int32 Fd = serialOpen(SerialPort);

    while(1) {

        Payload0.Info += 1;
        char EncodedMessage[HT_PROTOCOL_CALC_ENCODED_SIZE(sizeof(Payload0))];
        HT_PROTOCOL_InitMsg(&Message, 1, 2, 3, sizeof(Payload0), (void *) &Payload0, true);
        HT_PROTOCOL_EncodeMsg(EncodedMessage, &Message);
        serialWrite(Fd, EncodedMessage, HT_PROTOCOL_CALC_ENCODED_SIZE(sizeof(Payload0)));

        int32 PayloadNum = rand() % 3;

        switch(PayloadNum) {
            case 5: {
                Payload0.Info += 1;
                char EncodedMessage[HT_PROTOCOL_CALC_ENCODED_SIZE(sizeof(Payload0))];
                HT_PROTOCOL_InitMsg(&Message, 1, 2, 3, sizeof(Payload0), (void *) &Payload0, true);
                HT_PROTOCOL_EncodeMsg(EncodedMessage, &Message);
                serialWrite(Fd, EncodedMessage, HT_PROTOCOL_CALC_ENCODED_SIZE(sizeof(Payload0)));
                break;
            }
            /* case 1: {
                Payload1.Measure += 0.1;
                char EncodedMessage[HT_PROTOCOL_CALC_ENCODED_SIZE(sizeof(Payload0))];
                HT_PROTOCOL_InitMsg(&Message, 1, 2, 3, sizeof(Payload1), (void *) &Payload1, true);
                HT_PROTOCOL_EncodeMsg(EncodedMessage, &Message);
                serialWrite(Fd, EncodedMessage, HT_PROTOCOL_CALC_ENCODED_SIZE(sizeof(Payload1)));
                break;
            }
            case 2: {
                Payload2.ControlBits += 1;
                char EncodedMessage[HT_PROTOCOL_CALC_ENCODED_SIZE(sizeof(Payload0))];
                HT_PROTOCOL_InitMsg(&Message, 1, 2, 3, sizeof(Payload2), (void *) &Payload2, true);
                HT_PROTOCOL_EncodeMsg(EncodedMessage, &Message);
                serialWrite(Fd, EncodedMessage, HT_PROTOCOL_CALC_ENCODED_SIZE(sizeof(Payload2)));
                break;
            } */
            default:
                break;
        }

        // NOTIFY("Write Task running\n");

        usleep(1000000);
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
        usleep(1000);
    }

    return 0;
}
