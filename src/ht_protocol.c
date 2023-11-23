#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "ht_protocol.h"

/* Mapping of ASCII characters to hex values */
const uint8 ASCII_hashmap[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  !"#$%&'
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ()*+,-./
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // 01234567
    0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 89:;<=>?
    0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, // @ABCDEFG
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // HIJKLMNO
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // PQRSTUVW
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // XYZ[\]^_
    0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, // `abcdefg
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // hijklmno
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // pqrstuvw
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // xyz{|}~.
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ........
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // ........
};

/* This function will convert a Hexa str to the data specified and write it into varToBeWritten */
/* It will return the bytes written */
uint8 hexstr_converter(void* varToBeWritten, CustomTypes data_type, const char* str, uint16 data_index) {

    uint8 i, bytes_length;
    uint8 *aux_bytes = NULL;
    char aux_str[2];
    void *aux_ptr = NULL;

    /* Initialize data according to the type here */
    switch (data_type) {
        case INT8_DATA:
            aux_ptr = (int8*) varToBeWritten;
            bytes_length = 1;
            break;
        case UINT8_DATA:
            aux_ptr = (uint8*) varToBeWritten;
            bytes_length = 1;
            break;
        case INT16_DATA:
            aux_ptr = (int16*) varToBeWritten;
            bytes_length = 2;
            break;
        case UINT16_DATA:
            aux_ptr = (uint16*) varToBeWritten;
            bytes_length = 2;
            break;
        case INT32_DATA:
            aux_ptr = (int32*) varToBeWritten;
            bytes_length = 4;
            break;
        case UINT32_DATA:
            aux_ptr = (uint32*) varToBeWritten;
            bytes_length = 4;
            break;
        case INT64_DATA:
            aux_ptr = (int64*) varToBeWritten;
            bytes_length = 8;
            break;
        case UINT64_DATA:
            aux_ptr = (uint64*) varToBeWritten;
            bytes_length = 8;
            break;
        case FLOAT_DATA:
            aux_ptr = (float*) varToBeWritten;
            bytes_length = 4;
            break;
        case DOUBLE_DATA:
            aux_ptr = (double*) varToBeWritten;
            bytes_length = 8;
            break;
        default:
            return 0;
            break;
    }

    /* Alloc memory to read bytes from str */
    aux_bytes = (uint8*) malloc (bytes_length * sizeof(uint8));

    /* Check if we have memory for it */
    if ((aux_bytes == NULL) || (aux_ptr == NULL)) {
        // TODO: Error case. failed to memalloc
        return 0;
    }

    /* Proper bytes reading according to HT Protocol */
    for(i=0; i<bytes_length; i++) {
        aux_str[0] = str[data_index + (2*i)];
        aux_str[1] = str[data_index + (2*i) + 1];

        /* This is a BIG ENDIAN notation */
        aux_bytes[(bytes_length-i-1)] = (uint8) ((ASCII_hashmap[(uint8) aux_str[0]] << 4) | ASCII_hashmap[(uint8) aux_str[1]]);
    }

    /* Cast bytes read to the var expected */
    memcpy((char*) aux_ptr, aux_bytes, bytes_length);

    /* Free memory used and return */
    if (aux_bytes!=NULL) free(aux_bytes);

    return i;
}

/* This function will serialize a given var into str buffer as hexadecimal string */
/* Note: This will use system default endianess */
/*  e.g. decimal_to_hexstr(&1253775, UINT64_DATA, str) will write "000000000013218f" to str */
uint8 decimal_to_hexstr(void *var, CustomTypes data_type, char* str) {

    /* Specific union for double casting to hexa */
    union {
        long long i; /* 8 bytes */
        double    d; /* 8 bytes */
    } double_value;

    /* Specific union for float casting to hexa */
    union {
        long     i; /* 4 bytes */
        float    d; /* 4 bytes */
    } float_value;

    void* aux_ptr;
    uint8 bytes_length = 0;

    switch (data_type) {
        case INT8_DATA:
            aux_ptr = (int8*) var;
            /* This has to be converted to uint8 to be printed */
            sprintf(str, "%02" PRIX8, * (uint8*) aux_ptr);
            bytes_length = 1;
            break;
        case UINT8_DATA:
            aux_ptr = (uint8*) var;
            sprintf(str, "%02" PRIX8, * (uint8*) aux_ptr);
            bytes_length = 1;
            break;
        case INT16_DATA:
            aux_ptr = (int16*) var;
            sprintf(str, "%04" PRIX16, * (uint16*) aux_ptr);
            bytes_length = 2;
            break;
        case UINT16_DATA:
            aux_ptr = (uint16*) var;
            sprintf(str, "%04" PRIX16, * (uint16*) aux_ptr);
            bytes_length = 2;
            break;
        case INT32_DATA:
            aux_ptr = (int32*) var;
            sprintf(str, "%08" PRIX32, * (uint32*) aux_ptr);
            bytes_length = 4;
            break;
        case UINT32_DATA:
            aux_ptr = (uint32*) var;
            sprintf(str, "%08" PRIX32, * (uint32*) aux_ptr);
            bytes_length = 4;
            break;
        case INT64_DATA:
            aux_ptr = (int64*) var;
            sprintf(str, "%016" PRIX64, * (uint64*) aux_ptr);
            bytes_length = 8;
            break;
        case UINT64_DATA:
            aux_ptr = (uint64*) var;
            sprintf(str, "%016" PRIX64, * (uint64*) aux_ptr);
            bytes_length = 8;
            break;
        case FLOAT_DATA:
            aux_ptr = (float*) var;
            float_value.d = * (float*) aux_ptr;
            sprintf(str, "%08lX", float_value.i);
            bytes_length = 4;
            break;
        case DOUBLE_DATA:
            aux_ptr = (double*) var;
            double_value.d = * (double*) aux_ptr;
            sprintf(str, "%016llX", double_value.i);
            bytes_length = 8;
            break;
        default:
            // /* Invalid data_type */
            return 0;
            break;
    }

    return bytes_length*2;
}

/* This function will insert the CRC symbol into str input */
/*  and will return 1 (char written) */
uint8 insert_crc_symbol(char *str) {

    sprintf(str, "%c", HT_CRC_CHAR);
    return 1;
}

/* This function will calculate the CRC for HT Protocol */
/* It requires the string to be without the first char "$" until the crc char ":" */
/*  e.g. HT_crc8('01023100:', 9) = 0x7F */
uint8 HT_crc8(const char *data, uint16 len) {

    uint8 crc = 0x00;
    uint16 i;

    if (len == 0) {
        return crc;
    }

    for (i=0; i<len; i++) {
        crc ^= *data++;
        crc = crc & 0x80 ? (crc << 1) ^ 0xEB : crc << 1;
        crc = crc & 0x80 ? (crc << 1) ^ 0xEB : crc << 1;
        crc = crc & 0x80 ? (crc << 1) ^ 0xEB : crc << 1;
        crc = crc & 0x80 ? (crc << 1) ^ 0xEB : crc << 1;
        crc = crc & 0x80 ? (crc << 1) ^ 0xEB : crc << 1;
        crc = crc & 0x80 ? (crc << 1) ^ 0xEB : crc << 1;
        crc = crc & 0x80 ? (crc << 1) ^ 0xEB : crc << 1;
        crc = crc & 0x80 ? (crc << 1) ^ 0xEB : crc << 1;
    }

    return crc;
} /* End of HT_crc8() */


/* This function will prepend the start byte and the end byte of data, */
/*  according to HT Protocol */
void HT_write(const char *data, uint16 data_length) {

    char *tx_buff = NULL;

    if (data == NULL || data_length <= 10) {
        return;
    }

    tx_buff = (char*) malloc((data_length+2) * sizeof(char));
    if (tx_buff == NULL) {
        return;
    }

    sprintf(tx_buff, "%c", HT_HW_START_CHAR);
    sprintf(tx_buff+1, "%s", data);
    sprintf(tx_buff+1+data_length, "%c", HT_HW_END_CHAR);

    /* Send to serial */
    //serialWrite(globalData.serial_fd, tx_buff, data_length+2);
    printf("%s\n", tx_buff);

    /* Free memory used */
    if (tx_buff != NULL) free(tx_buff);
}
