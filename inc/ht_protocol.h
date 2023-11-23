#include <stdint.h>
#include "common_types.h"

#ifndef HTPROTOCOL_H
#define HTPROTOCOL_H

/* This has to change if the CRC is disabled */
#define HT_MIN_PACKET_CHARS                      14 // {"src_address": 1, "dest_address": 1, "cmd_id": 1, "data_length": 4, "crc_sep": 1, "crc": 1}
#define HT_DATA_START                            10 // Where the payload data starts in the HT protocol
#define HT_HW_START_CHAR                         '$'
#define HT_HW_END_CHAR                           '\n'
#define HT_CRC_CHAR                              ':'

#define HT_MAX_MESSAGE_BYTES                     2048
#define HT_SOURCE_ADDR                           0x01
#define HT_DEST_ADDR                             0x18

#endif