#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include "common_types.h"
#include "serial.h"
#include "ht_protocol.h"

// Just an example of a read thread that will read from the serial port
void* read_serial_task(void* arg) {

    int32 bytes_read = 0;
    static char str_buffer[HT_MAX_MESSAGE_BYTES];

    log_to_file("Starting read_serial task\n");

    while(1) {

        // Removed globalData here
        //bytes_read = serialRead(globalData.serial_fd, str_buffer, HT_HW_START_CHAR, HT_HW_END_CHAR, 0, HT_MAX_MESSAGE_BYTES);
        bytes_read = serialRead(0, str_buffer, HT_HW_START_CHAR, HT_HW_END_CHAR, 0, HT_MAX_MESSAGE_BYTES);

        if (bytes_read <= 0) {
            //globalData.hw_failures++;
        } else {
            /* Message read. Process it */
            process_read_msg(str_buffer, (uint16) bytes_read);
        }
    }

    return NULL;
}

/* This function is to be used on the Reading Task */
/* It will process a given telemetry read and send it to the SW Bus */
/* str Must be already without the Start and End bytes (i.e: "20304050" and NOT "$20304050\n") */
void process_read_msg(const char *str, uint16 str_len) {

    uint8 source_address, dest_address, command_id;
    uint16 packet_data_length;

    /* Verify if str_len is the minimun required to read it's data size */
    if ((str == NULL) || (str_len < HT_MIN_PACKET_CHARS)) {
        /* This is not a valid message */
        return;
    }

    /* Get basic packet info (Source Address, Dest Address, Command ID, Data Length) */
    hexstr_converter(&source_address, UINT8_DATA, str, 0);
    hexstr_converter(&dest_address, UINT8_DATA, str, 2);
    hexstr_converter(&command_id, UINT8_DATA, str, 4);
    hexstr_converter(&packet_data_length, UINT16_DATA, str, 6);

    /* Perform some validations */
    /* Validate CRC */
    if (validate_crc(str, str_len) != 0) {
        /* Wrong CRC. Discard packet? */
        return;
    }

    /* Validate Dest Address */
    if (dest_address != HT_SOURCE_ADDR) {
        /* Wrong Dest Address. Discard */
        return;
    }

    /* Validate Packet Length */
    if (str_len != ((packet_data_length*2) + HT_MIN_PACKET_CHARS)) {

        /* Wrong packet length and actual length */
        return;
    }

    /* Past this point, we are safe to create a packet */
    /* Each function will automatically send the packet to the SW Bus */
    switch (command_id) {
        case 0x47: // suppose 0x47 = CMD_GET_HK
            read_frame_packet(str);
            break;
        default:
            /* Invalid command_id. Probably Error */
            break;
    }
}