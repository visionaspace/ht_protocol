#ifndef HT_PROTOCOL_INTERNAL_H
#define HT_PROTOCOL_INTERNAL_H

/* Include all external/public definitions */
#include "ht_protocol.h"
#include <stdint.h>

#define HT_HW_START_CHAR    '$'
#define HT_HW_END_CHAR      '\n'
#define HT_CRC_CHAR         ':'

#define HT_MAX_MESSAGE_BYTES 2048

/*************************************************************************
** Function Declarations
*************************************************************************/

/**
 * @brief Transforms decimal values into hexstrings
 *
 * @param var Value to be converted
 * @param DataType Type of the value to be converted
 * @param str pointer to the string where the hexstring will be stored
 * @return bytes written
 */
int16 HT_PROTOCOL_DecimalToHexstr(void *Var, HT_PROTOCOL_CustomTypes_t DataType, char *Str);

/**
 * @brief Computes the CRC8 value of a given data
 *
 * @param Data The data to be computed
 * @param Len The length of the data
 * @return Returns the computed CRC8 value
 */
uint8 HT_PROTOCOL_Crc8(const char *Data, size_t Len);

/**
 * @brief Checks if a string is an HT message
 *
 * @param Msg Pointer to the buffer to be checked
 * @return True if the message is an HT message / false otherwise
 */
bool HT_PROTOCOL_IsMsgHt(void *Msg);

/**
 * @brief Encodes arbitrary data into a string, byte-by-byte
 *
 * @param EncodedMsg Pointer to the buffer where the encoded message will be stored.
 * @param Data Pointer to the buffer where the message is stored.
 * @param NumOfBytes The length of the message.
 */
size_t HT_PROTOCOL_EncBytes(char *EncodedMsg, void *Msg, size_t NumOfBytes);

/**
 * @brief This function decodes a string of arbitrary length, byte-by-byte, into the provided DecodedMsg pointer
 *
 * @param DecodedMsg Pointer to the buffer where the decoded message will be stored
 * @param Msg Pointer to the buffer where the encoded message is stored
 * @param NumOfBytes Length of the *encoded* message
 * @return The number of bytes written to the DecodedMsg buffer
 */
size_t HT_PROTOCOL_DecBytes(void *DecodedMsg, void *Msg, size_t NumOfBytes);

#endif

