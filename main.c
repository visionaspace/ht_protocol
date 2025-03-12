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
#include "helpers.h"

const char *SerialPort = "/dev/your_device";

void ReadTask(void) {

#define CMD_PAYLOAD_EXAMPLE_0_CMD_ID 0
    typedef struct __attribute__((packed)) {
        uint8  Data;
        uint16 Info;
        uint8  Signal;
    } CmdPayloadExample0_t;

#define CMD_PAYLOAD_EXAMPLE_1_CMD_ID 1
    typedef struct __attribute__((packed)) {
        uint16 Data;
        uint16 Info;
        double Signal;
    } CmdPayloadExample1_t;


#define CMD_PAYLOAD_EXAMPLE_2_CMD_ID 2
    typedef struct __attribute__((packed)) {
        char  Data[20];
        float Signal;
    } CmdPayloadExample2_t;

    int32 bytes_read = 0;

    static char Buffer[HT_MAX_MESSAGE_BYTES];

    int32 Fd = serialOpen(SerialPort);

    while(1) {
        bytes_read = serialRead(Fd, Buffer, HT_HW_START_CHAR, HT_HW_END_CHAR, SERIAL_INCLUDE_MARKETS, HT_MAX_MESSAGE_BYTES);

        if (bytes_read <= 0) {
            // READ_TASK_NOTIFY(No bytes read on serial device\n");
        } else {
            switch(HT_PROTOCOL_GetCmdId(Buffer)) {
                case CMD_PAYLOAD_EXAMPLE_0_CMD_ID: {
                    RECEIVE_MSG(CmdPayloadExample0_t, CMD_PAYLOAD_EXAMPLE_0_CMD_ID, {
                        READ_TASK_NOTIFY("Payload0 = (%u, %u, %u)\n", Payload.Data, Payload.Info, Payload.Signal);
                    });
                    break;
                }
                case CMD_PAYLOAD_EXAMPLE_1_CMD_ID: {
                    RECEIVE_MSG(CmdPayloadExample1_t, CMD_PAYLOAD_EXAMPLE_1_CMD_ID, {
                        READ_TASK_NOTIFY("Payload1 = (%u, %u, %lf)\n", Payload.Data, Payload.Info, Payload.Signal);
                    });
                    break;
                }
                case CMD_PAYLOAD_EXAMPLE_2_CMD_ID: {
                    RECEIVE_MSG(CmdPayloadExample2_t, CMD_PAYLOAD_EXAMPLE_2_CMD_ID, {
                        READ_TASK_NOTIFY("Payload2 = (%s, %f)\n", Payload.Data, Payload.Signal);
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

#define TLM_PAYLOAD_EXAMPLE_0_CMD_ID 0
    typedef struct {
        uint8  Info;
        uint8  Data;
        uint16 Temp;
        float  Pressure;
    } TlmPayloadExample0_t;

#define TLM_PAYLOAD_EXAMPLE_1_CMD_ID 1
    typedef struct {
        double Measure;
    } TlmPayloadExample1_t;

#define TLM_PAYLOAD_EXAMPLE_2_CMD_ID 2
    typedef struct {
        uint8 ControlBits;
    } TlmPayloadExample2_t;

    TlmPayloadExample0_t Payload0 = {.Info = 0x01};
    TlmPayloadExample1_t Payload1 = {.Measure = 0x05};
    TlmPayloadExample2_t Payload2 = {.ControlBits = 0x06};

    HT_PROTOCOL_Msg_t Message;

    int32 Fd = serialOpen(SerialPort);

    while(1) {
        for (uint32 PayloadNum = 0; PayloadNum <= TLM_PAYLOAD_EXAMPLE_2_CMD_ID; PayloadNum++) {
            switch(PayloadNum) {
                case TLM_PAYLOAD_EXAMPLE_0_CMD_ID: {
                    SEND_MSG(Payload0, TLM_PAYLOAD_EXAMPLE_0_CMD_ID, {
                        SAFE_INCR(Payload0.Info);
                        SAFE_INCR(Payload0.Data);
                        SAFE_INCR(Payload0.Temp);
                        Payload0.Pressure += 0.1;
                    });
                   break;
                }
                case TLM_PAYLOAD_EXAMPLE_1_CMD_ID: {
                    SEND_MSG(Payload1, TLM_PAYLOAD_EXAMPLE_1_CMD_ID, {
                        Payload1.Measure += 0.1;
                    });
                   break;
                }
                case TLM_PAYLOAD_EXAMPLE_2_CMD_ID: {
                    SEND_MSG(Payload2, TLM_PAYLOAD_EXAMPLE_2_CMD_ID, {
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
        perror("Error creating read task\n");
        return -1;
    }

    if (pthread_create(&WriteTaskThread, NULL, (void *)&WriteTask, NULL) != 0) {
        perror("Error creating write task\n");
        return -1;
    }

    pthread_join(ReadTaskThread, NULL);
    pthread_join(WriteTaskThread, NULL);

    while(1) {
        usleep(100000000);
    }

    return 0;
}
