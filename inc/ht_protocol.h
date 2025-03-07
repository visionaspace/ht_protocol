#ifndef HT_PROTOCOL_H
#define HT_PROTOCOL_H

#include <string.h>
#include <stdlib.h>
#include "common_types.h"
#include "ht_protocol_types.h"
#include "ht_protocol_error.h"
#include "ht_protocol_internal.h"

typedef struct {
    uint8  SrcId;
    uint8  DestId;
    uint8  CmdId;
    uint16 DataLength;
} HT_PROTOCOL_MsgHeader_t;

typedef struct {
    HT_PROTOCOL_MsgHeader_t Header;
    uint8              CrcValue;
    uint8              UseCrc;
    void              *Data;
} HT_PROTOCOL_Msg_t;

#define HT_PROTOCOL_NUM_CONTROL_CHARS (3)

/*
 * The correct value returned by the HT_PROTOCOL_CALC_ENCODED_SIZE macro should actually be
 * (DataSize + sizeof(HT_PROTOCOL_MsgHeader_t) + 1) * 2 + HT_PROTOCOL_NUM_CONTROL_CHARS;
 *  - DataSize is the actually sizeof of the data to be encoded
 *  - sizeof(HT_PROTOCOL_MsgHeader_t) is the size of the header struct (which, in theory, should be 5 bytes)
 *  - 1 is the size of the CRC byte when 'flattened' into the encoded buffer
 *  - HT_PROTOCOL_NUM_CONTROL_CHARS is the number of control characters that are added to the encoded buffer by the protocol itself; those being
 *    start byte, CRC byte and end byte.
 *
 *  However, the current implementation of the HT_PROTOCOL_MsgHeader has a hidden Spare byte between its CmdId and DataLength fields, which makes it so
 *  that it is actually 6 bytes long. This way, we do not need to add the "+1" to the macro, as the CRC byte is already accounted for. That being said,
 *  the macro is defined as follows.
 * */
#define HT_PROTOCOL_CALC_ENCODED_SIZE(DataSize) ((DataSize + sizeof(HT_PROTOCOL_MsgHeader_t)) * 2 + HT_PROTOCOL_NUM_CONTROL_CHARS)

/**
 * @brief Initializes the HT library.
 *
 * @return 0 on success, negative error code in case of error
 */
int32 HT_PROTOCOL_Init(void);

/**
 * @brief This function initializes an HT message.
 *
 * @param Msg Pointer to the struct where the message will be stored
 * @param SrcId The source ID of the message
 * @param DestId The destination ID of the message
 * @param CmdId The command ID of the message
 * @param DataLength The length of the data in the message
 * @param Data Pointer to the payload of the message
 * @param UseCrc A boolean flag to indicate if CRC should be added to the message
 * @return 0 on success, negative errr code in case of error
 */
int32 HT_PROTOCOL_InitMsg(HT_PROTOCOL_Msg_t *Msg, uint8 SrcId, uint8 DestId, uint8 CmdId, uint16 PayloadSize, void *Payload, bool UseCrc);

/**
 * @brief Encodes an HT_PROTOCOL_Msg_t struct into an HT-encoded string
 *
 * @param EncodedMsg String pointer where the ht message should be encoded
 * @param Msg The HT_PROTOCOL_Msg_t struct containing the message elements
 * @return Negative error code in case of error, number of bytes written otherwise
 */
int32 HT_PROTOCOL_EncodeMsg(char *EncodedMsg, HT_PROTOCOL_Msg_t *Msg);

/**
 * @brief Decodes an HT message into an HT_PROTOCOL_Msg_t type
 *
 * @param DecodedMsg Pointer to where the decoded struct will be stored
 * @param DecodedPayload Pointer to where to store the decoded payload
 * @param DecodedPayloadSize Size allocated for the payload
 * @param EncodedMsg Pointer to the encoded HT message
 * @return Negative error code in case of error, number of bytes written otherwise
 */
int32 HT_PROTOCOL_DecodeMsg(HT_PROTOCOL_Msg_t *Msg, void *DecodedPayload, size_t DecodedPayloadSize, char *EncodedMsg);

/**
 * @brief Converts a hexstring into an int or float data type
 *
 * @param VarToBeWritten Pointer to where to write the values to
 * @param DataType Type of data (UINT8_DATA, INT32_DATA, etc.)
 * @param Str Pointer containing the hexstring
 * @param DataIndex Which index of the string should be converted
 * @return Number of bytes written
 */
int16 HT_PROTOCOL_HexstrConverter(void *VarToBeWritten, HT_PROTOCOL_CustomTypes_t DataType, const char *Str, uint16 DataIndex);

/**
 * @brief Get Source ID from an encoded HT Message
 *
 * @param Msg The encoded message
 * @return In case of success, returns the Source ID (uint8). Otherwise, returns a negative error code.
 */
int32 HT_PROTOCOL_GetSrcId(char *Msg);

/**
 * @brief Get Destination ID from an encoded HT Message
 *
 * @param Msg The encoded message
 * @return In case of success, returns the Destination ID (uint8). Otherwise, returns a negative error code.
 */
int32 HT_PROTOCOL_GetDestId(char *Msg);

/**
 * @brief Get Command ID from an encoded HT Message
 *
 * @param Msg The encoded message
 * @return In case of success, returns the Command ID (uint8). Otherwise, returns a negative error code.
 */
int32 HT_PROTOCOL_GetCmdId(char *Msg);

/**
 * @brief Get the reported data length from an encoded HT Message
 *
 * @param Msg The encoded message
 * @return In case of success, returns the Data Length (uint16). Otherwise, returns a negative error code.
 */
int32 HT_PROTOCOL_GetMsgLength(char *Msg);

/**
 * @brief Decodes only the payload of a given encoded HT message
 *
 * @param DecodedPayload Pointer to where the payload should be decoded to
 * @param DecodedPayloadSize Number of bytes allocated for the payload
 * @param EncodedMsg Pointer to the encoded message
 * @return Negative error code in case of error, number of bytes written otherwise
 */
int32 HT_PROTOCOL_DecodePayload(void *DecodedPayload, size_t DecodedPayloadSize, char *EncodedMsg);

#endif
