#ifndef _VLOG_
#define _VLOG_

/* Colors codes. */
#define _VLOG_WHITE_CODE    "\033[0m"
#define _VLOG_GREEN_CODE    "\033[22;32m"
#define _VLOG_YELLOW_CODE   "\033[22;33m"
#define _VLOG_RED_CODE      "\033[22;31m"
#define _VLOG_BLUE_CODE     "\033[22;34m"

/* Labels. */
#define VLOG_MSG_LABEL        "[MSG]"
#define VLOG_CMD_LABEL        "[CMD]"
#define VLOG_TLM_LABEL        "[TLM]"
#define VLOG_ERR_LABEL        "[ERR]"
#define VLOG_BACKEND_LABEL    "[BCKND]"
#define VLOG_META_LABEL       "[META]"
#define VLOG_WRITE_TASK_LABEL "[WRITE_TASK]"
#define VLOG_READ_TASK_LABEL  "[READ_TASK]"


#define VLOG_STR_TIME_SIZE  (14)

/* Hash-like enum for log usage. */
typedef enum {
    VLOG_WHITE,
    VLOG_GREEN,
    VLOG_YELLOW,
    VLOG_RED,
    VLOG_BLUE
} VLOG_Colors_t;

/* Headers */
char * VLOG_GetFormattedTime(void);
void VLOG_Debug(VLOG_Colors_t, const char *, const char *, const char * , ...);

#define VLOG_MSG(_format, ...)        VLOG_Debug(VLOG_WHITE,  VLOG_GetFormattedTime(), VLOG_MSG_LABEL,        _format, ##__VA_ARGS__)
#define VLOG_CMD(_format, ...)        VLOG_Debug(VLOG_GREEN,  VLOG_GetFormattedTime(), VLOG_CMD_LABEL,        _format, ##__VA_ARGS__)
#define VLOG_TLM(_format, ...)        VLOG_Debug(VLOG_GREEN,  VLOG_GetFormattedTime(), VLOG_TLM_LABEL,        _format, ##__VA_ARGS__)
#define VLOG_CMD_WARN(_format, ...)   VLOG_Debug(VLOG_YELLOW, VLOG_GetFormattedTime(), VLOG_CMD_LABEL,        _format, ##__VA_ARGS__)
#define VLOG_META_MSG(_format, ...)   VLOG_Debug(VLOG_YELLOW, VLOG_GetFormattedTime(), VLOG_META_LABEL,       _format, ##__VA_ARGS__)
#define VLOG_ERR(_format, ...)        VLOG_Debug(VLOG_RED,    VLOG_GetFormattedTime(), VLOG_ERR_LABEL,        _format, ##__VA_ARGS__)
#define VLOG_NOTIFY(_format, ...)     VLOG_Debug(VLOG_BLUE,   VLOG_GetFormattedTime(), VLOG_BACKEND_LABEL,    _format, ##__VA_ARGS__)
#define VLOG_READ_TASK(_format, ...)  VLOG_Debug(VLOG_GREEN,  VLOG_GetFormattedTime(), VLOG_READ_TASK_LABEL,  _format, ##__VA_ARGS__)
#define VLOG_WRITE_TASK(_format, ...) VLOG_Debug(VLOG_BLUE,  VLOG_GetFormattedTime(), VLOG_WRITE_TASK_LABEL, _format, ##__VA_ARGS__)

#endif
