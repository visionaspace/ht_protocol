#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <vlog.h>

char vlog_time_str[VLOG_STR_TIME_SIZE];

/* Util function to access current time for better naming of the emulators logs. */
char * VLOG_GetFormattedTime(void) {

    time_t raw_time;
    struct tm * time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);
    snprintf(
        vlog_time_str, VLOG_STR_TIME_SIZE, "[%02d:%02d:%02d]",
        time_info->tm_hour, time_info->tm_min, time_info->tm_sec
    );

    return (char *) vlog_time_str;
}

/*
** Log function, used for proper identification when logging different kinds of events.
**     @color   - represents the color code related to the log.
**     @timetag - string formatted as a time identifier,
**     @label   - start of the message for proper differentiation inside the same log color.
**     @format  - printf-like formated string.
*/
void VLOG_Debug(VLOG_Colors_t color, const char * timetag, const char * label, const char * format, ...) {

    va_list args;

    /* If the timetag or the label are NULL, do not log. */
    if (timetag == NULL || label == NULL) {
        return;
    }

    /*
    ** Compile time case to ensure different log messages while also changing
    ** the STDOUT color.
    */
    switch (color) {
        #define VLOG_CASE(_color_case, _color)             \
            case _color_case: {                            \
                printf("%s%s%s ", timetag, label, _color); \
                break;                                     \
            }
        VLOG_CASE(VLOG_WHITE,   _VLOG_WHITE_CODE);
        VLOG_CASE(VLOG_GREEN,   _VLOG_GREEN_CODE);
        VLOG_CASE(VLOG_YELLOW,  _VLOG_YELLOW_CODE);
        VLOG_CASE(VLOG_RED,     _VLOG_RED_CODE);
        VLOG_CASE(VLOG_BLUE,    _VLOG_BLUE_CODE);
        default:
            return;
        #undef VLOG_CASE
    }

    /* Use the va_list arguments. */
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    /* Then, finally, reset the STDOUT color. */
    printf("%s", _VLOG_WHITE_CODE);

    return;
}
