#ifndef HT_PROTOCOL_TYPES_H
#define HT_PROTOCOL_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include "common_types.h"

typedef bool boolean;

typedef enum {
    BOOLEAN_DATA,
    CHAR_DATA,
    INT8_DATA,
    UINT8_DATA,
    INT16_DATA,
    UINT16_DATA,
    INT32_DATA,
    UINT32_DATA,
    INT64_DATA,
    UINT64_DATA,
    FLOAT_DATA,
    DOUBLE_DATA
} HT_PROTOCOL_CustomTypes_t;

#endif

