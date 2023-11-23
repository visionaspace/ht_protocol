#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include "ht_protocol.h"

/* This will write all commands that has 0 byte payload */
void HT_write_0_payload(uint8 cmdID) {

    char data[HT_MIN_PACKET_CHARS];
    uint16 index = 0;

    uint8 sourceAddr = HT_SOURCE_ADDR;
    uint8 destAddr = HT_DEST_ADDR;
    uint8 crc = 0;
    uint16 dataLength = 0;

    index += decimal_to_hexstr((void*) &sourceAddr, UINT8_DATA, data+index);
    index += decimal_to_hexstr((void*) &destAddr, UINT8_DATA, data+index);
    index += decimal_to_hexstr((void*) &cmdID, UINT8_DATA, data+index);
    index += decimal_to_hexstr((void*) &dataLength, UINT16_DATA, data+index);

    /* Calculate crc and finish */
    index += insert_crc_symbol(data+index);
    crc = HT_crc8(data, index);
    index += decimal_to_hexstr((void*) &crc, UINT8_DATA, data+index);

    HT_write(data, index);
}

/* This will write a command with uint32 param */
void HT_write_set_param(uint8 cmdID, uint32 param) {

    char data[HT_MIN_PACKET_CHARS + (4*2)];
    uint16 index = 0;

    uint8 sourceAddr = HT_SOURCE_ADDR;
    uint8 destAddr = HT_DEST_ADDR;
    uint8 crc = 0;
    uint16 dataLength = 4;

    index += decimal_to_hexstr((void*) &sourceAddr, UINT8_DATA, data+index);
    index += decimal_to_hexstr((void*) &destAddr, UINT8_DATA, data+index);
    index += decimal_to_hexstr((void*) &cmdID, UINT8_DATA, data+index);
    index += decimal_to_hexstr((void*) &dataLength, UINT16_DATA, data+index);

    /* Param payload */
    index += decimal_to_hexstr((void*) &param, UINT32_DATA, data+index);

    /* Calculate crc and finish */
    index += insert_crc_symbol(data+index);
    crc = HT_crc8(data, index);
    index += decimal_to_hexstr((void*) &crc, UINT8_DATA, data+index);

    HT_write(data, index);
}

void main() {
    HT_write_0_payload(0x17);
    HT_write_set_param(0x21, 12345);
}