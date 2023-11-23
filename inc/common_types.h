#include <stdint.h>
#include <stdbool.h>

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

typedef int8_t                                int8;
typedef int16_t                               int16;
typedef int32_t                               int32;
typedef int64_t                               int64;
typedef uint8_t                               uint8;
typedef uint16_t                              uint16;
typedef uint32_t                              uint32;
typedef uint64_t                              uint64;
typedef bool                                  boolean;

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
} CustomTypes;

#endif // COMMON_TYPES_H

