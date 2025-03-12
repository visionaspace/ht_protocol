#ifndef _HELPERS_
#define _HELPERS_

pthread_mutex_t IOMutex;

#define WRITE_TASK_NOTIFY(_format, ...)     \
{                                           \
    pthread_mutex_lock(&IOMutex);           \
    VLOG_WRITE_TASK(_format, ##__VA_ARGS__);\
    pthread_mutex_unlock(&IOMutex);         \
}                                           \

#define READ_TASK_NOTIFY(_format, ...)      \
{                                           \
    pthread_mutex_lock(&IOMutex);           \
    VLOG_READ_TASK(_format, ##__VA_ARGS__); \
    pthread_mutex_unlock(&IOMutex);         \
}                                           \

#define MAX_FOR(type) (1 << sizeof(type))

#define SAFE_INCR(var) var = (var == MAX_FOR(var)) ? 1 : var + 1

#define SEND_MSG(Payload, CmdId, Scope) \
{                                                                                          \
    char EncodedMessage[HT_PROTOCOL_CALC_ENCODED_SIZE(sizeof(Payload))];                   \
    {Scope};                                                                               \
    HT_PROTOCOL_InitMsg(&Message, 1, 2, CmdId, sizeof(Payload), (void *) &Payload, true);  \
    HT_PROTOCOL_EncodeMsg(EncodedMessage, &Message);                                       \
    serialWrite(Fd, EncodedMessage, HT_PROTOCOL_CALC_ENCODED_SIZE(sizeof(Payload)));       \
    WRITE_TASK_NOTIFY("Wrote (%s) = %s", #Payload, EncodedMessage);                        \
}                                                                                          \

#define RECEIVE_MSG(PayloadType, CmdId, Scope) \
{                                                                           \
        HT_PROTOCOL_Msg_t Message;                                          \
        PayloadType Payload;                                                \
        HT_PROTOCOL_DecodeMsg(&Message, &Payload, sizeof(Payload), Buffer); \
        {Scope}                                                             \
}                                                                           \

#endif
