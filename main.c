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

#define SEND_MSG(Payload, CmdId, Scope) \
{                                                                                          \
    char EncodedMessage[HT_PROTOCOL_CALC_ENCODED_SIZE(sizeof(Payload))];                   \
    {Scope};                                                                               \
    HT_PROTOCOL_InitMsg(&Message, 1, 2, CmdId, sizeof(Payload), (void *) &Payload, true); \
    HT_PROTOCOL_EncodeMsg(EncodedMessage, &Message);                                       \
    serialWrite(Fd, EncodedMessage, HT_PROTOCOL_CALC_ENCODED_SIZE(sizeof(Payload)));       \
    WRITE_TASK_NOTIFY("Wrote message for (%s) = %s", #Payload, EncodedMessage);            \
}                                                                                          \

const char *SerialPort = "/dev/pts/9";

void ReadTask(void) {

    typedef struct __attribute__((packed)) {
        uint8  Data;
        uint16 Info;
        uint8  Signal;
    } PayloadCmdExample0_t;

    typedef struct __attribute__((packed)) {
        uint16 Data;
        uint16 Info;
        double Signal;
    } PayloadCmdExample1_t;

    int32 bytes_read = 0;
    static char Buffer[HT_MAX_MESSAGE_BYTES];

    int32 Fd = serialOpen(SerialPort);

    while(1) {
        // bytes_read = serialRead(Fd, Buffer, HT_HW_START_CHAR, HT_HW_END_CHAR, 0, HT_MAX_MESSAGE_BYTES);
        bytes_read = serialRead(Fd, Buffer, HT_HW_START_CHAR, HT_HW_END_CHAR, SERIAL_INCLUDE_MARKETS, HT_MAX_MESSAGE_BYTES);

        if (bytes_read <= 0) {
            // READ_TASK_NOTIFY("Error reading serial\n");
        } else {
            /* int32 SrcId = HT_PROTOCOL_GetSrcId(Buffer);
            int32 DestId = HT_PROTOCOL_GetDestId(Buffer);
            int32 DataLength = HT_PROTOCOL_GetMsgLength(Buffer); */

            // int32 CmdId = HT_PROTOCOL_GetCmdId(Buffer);

#define RECEIVE_MSG(PayloadType, CmdId, Scope) \
{                                                                           \
        HT_PROTOCOL_Msg_t Message;                                          \
        PayloadType Payload;                                                \
        HT_PROTOCOL_DecodeMsg(&Message, &Payload, sizeof(Payload), Buffer); \
        {Scope}                                                             \
}                                                                           \

            switch(HT_PROTOCOL_GetCmdId(Buffer)) {
                case 0: {
                    RECEIVE_MSG(PayloadCmdExample0_t, 0, {
                        READ_TASK_NOTIFY("Payload0 = (%u, %u, %u)\n", Payload.Data, Payload.Info, Payload.Signal);
                    });
                    break;
                }
                case 1: {
                    RECEIVE_MSG(PayloadCmdExample1_t, 1, {
                        READ_TASK_NOTIFY("Payload1 = (%u, %u, %lf)\n", Payload.Data, Payload.Info, Payload.Signal);
                    });
                    break;
                }
                default:
                    break;
            }
        }
        usleep(500000);
    }
}

void WriteTask(void) {

    #define PAYLOAD_0_CMD_ID 0
    typedef struct {
        uint8  Info;
        uint8  Data;
        uint16 Temp;
        float  Pressure;
    } PayloadExample0_t;

    #define PAYLOAD_1_CMD_ID 1
    typedef struct {
        double Measure;
    } PayloadExample1_t;

    #define PAYLOAD_2_CMD_ID 2
    typedef struct {
        uint8 ControlBits;
    } PayloadExample2_t;

    PayloadExample0_t Payload0 = {.Info = 0x01};
    PayloadExample1_t Payload1 = {.Measure = 0x05};
    PayloadExample2_t Payload2 = {.ControlBits = 0x06};

    HT_PROTOCOL_Msg_t Message;

    int32 Fd = serialOpen(SerialPort);

    while(1) {

        for (uint32 PayloadNum = 0; PayloadNum < 3; PayloadNum++) {
            switch(PayloadNum) {
                case PAYLOAD_0_CMD_ID: {
                    SEND_MSG(Payload0, PAYLOAD_0_CMD_ID, {
                        SAFE_INCR(Payload0.Info);
                        SAFE_INCR(Payload0.Data);
                        SAFE_INCR(Payload0.Temp);
                        Payload0.Pressure += 0.1;
                    });
                   break;
                }
                case PAYLOAD_1_CMD_ID: {
                    SEND_MSG(Payload1, PAYLOAD_1_CMD_ID, {
                        Payload1.Measure += 0.1;
                    });
                   break;
                }
                case PAYLOAD_2_CMD_ID: {
                    SEND_MSG(Payload2, PAYLOAD_2_CMD_ID, {
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
