#include "ht_protocol_internal.h"
#include "ht_protocol_error.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "ht_protocol.h"

/*************************************************************************
** Global Data Structures
*************************************************************************/

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

/**
 * @file ht_lib.h
 */
int16 HT_PROTOCOL_HexstrConverter(void *VarToBeWritten, HT_PROTOCOL_CustomTypes_t DataType, const char *Str, uint16 DataIndex) {

    uint8 i, BytesLength;
    uint8 AuxBytes[8];
    char  AuxStr[2];
    void *AuxPtr = NULL;

    /* Initialize data according to the type here */
    switch (DataType) {
#define CASE_DATA_TYPE(_type_case, _type) \
        case _type_case: {                          \
            AuxPtr      = (_type *) VarToBeWritten; \
            BytesLength = sizeof(_type);            \
            break;                                  \
        }
        CASE_DATA_TYPE(INT8_DATA,   int8);
        CASE_DATA_TYPE(UINT8_DATA,  uint8);
        CASE_DATA_TYPE(INT16_DATA,  int16);
        CASE_DATA_TYPE(UINT16_DATA, uint16);
        CASE_DATA_TYPE(INT32_DATA,  int32);
        CASE_DATA_TYPE(UINT32_DATA, uint32);
        CASE_DATA_TYPE(INT64_DATA,  int64);
        CASE_DATA_TYPE(UINT64_DATA, uint64);
        CASE_DATA_TYPE(FLOAT_DATA,  float);
        CASE_DATA_TYPE(DOUBLE_DATA, double);
#undef CASE_DATA_TYPE
        default:
            return HT_ERROR_INVALID_DATA_TYPE;
    }

    /* Proper bytes reading according to HT Protocol */
    for (i = 0; i < BytesLength; i++) {
        AuxStr[0] = Str[DataIndex + (2 * i)];
        AuxStr[1] = Str[DataIndex + (2 * i) + 1];

        /* This is a BIG ENDIAN notation */
        AuxBytes[(BytesLength - i - 1)] = (uint8) ((ASCII_hashmap[(uint8) AuxStr[0]] << 4) | ASCII_hashmap[(uint8) AuxStr[1]]);
    }

    /* Cast bytes read to the var expected */
    memcpy((char *) AuxPtr, AuxBytes, BytesLength);

    return i;
}

/**
 * @file ht_lib.h
 */
int32 HT_PROTOCOL_GetSrcId(char *Msg) {

    if (Msg == NULL) {
        return HT_ERROR_NULL_POINTER;
    }

    if (HT_PROTOCOL_IsMsgHt(Msg) == false) {
        return HT_ERROR_NOT_HT_MSG;
    }

    char *Ptr   = (char *) Msg;
    uint8 SrcId = 0;

    /* Not handling error case for hard coded value */
    HT_PROTOCOL_HexstrConverter(&SrcId, UINT8_DATA, Ptr, 1);
    return SrcId;
}

/**
 * @file ht_lib.h
 */
int32 HT_PROTOCOL_GetDestId(char *Msg) {

    if (Msg == NULL) {
        return HT_ERROR_NULL_POINTER;
    }

    if (HT_PROTOCOL_IsMsgHt(Msg) == false) {
        return HT_ERROR_NOT_HT_MSG;
    }

    char *Ptr = (char *) Msg;

    uint8 DestId = 0;

    /* Not handling error case for hard coded value */
    HT_PROTOCOL_HexstrConverter(&DestId, UINT8_DATA, Ptr, 3);
    return DestId;
}

/**
 * @file ht_lib.h
 */
int32 HT_PROTOCOL_GetCmdId(char *Msg) {

    if (Msg == NULL) {
        return HT_ERROR_NULL_POINTER;
    }

    if (HT_PROTOCOL_IsMsgHt(Msg) == false) {
        return HT_ERROR_NOT_HT_MSG;
    }

    char *Ptr   = (char *) Msg;
    uint8 CmdId = 0;

    /* Not handling error case for hard coded value */
    HT_PROTOCOL_HexstrConverter(&CmdId, UINT8_DATA, Ptr, 5);
    return CmdId;
}

/**
 * @file ht_lib.h
 */
int32 HT_PROTOCOL_GetMsgLength(char *Msg) {

    if (Msg == NULL) {
        return HT_ERROR_NULL_POINTER;
    }

    if (HT_PROTOCOL_IsMsgHt(Msg) == false) {
        return HT_ERROR_NOT_HT_MSG;
    }

    char  *Ptr    = (char *) Msg;
    uint16 MsgLen = 0;

    /* Not handling error case for hard coded value */
    HT_PROTOCOL_HexstrConverter(&MsgLen, UINT16_DATA, Ptr, 7);
    return MsgLen;
}

/**
 * @file ht_lib_internal.h
 */
uint8 HT_PROTOCOL_Crc8(const char *Data, size_t Len) {

    uint8  Crc = 0x00;
    uint16 i;

    if (Len == 0) {
        return Crc;
    }

    for (i = 0; i < Len; i++) {
        Crc ^= *Data++;
        Crc = Crc & 0x80 ? (Crc << 1) ^ 0xEB : Crc << 1;
        Crc = Crc & 0x80 ? (Crc << 1) ^ 0xEB : Crc << 1;
        Crc = Crc & 0x80 ? (Crc << 1) ^ 0xEB : Crc << 1;
        Crc = Crc & 0x80 ? (Crc << 1) ^ 0xEB : Crc << 1;
        Crc = Crc & 0x80 ? (Crc << 1) ^ 0xEB : Crc << 1;
        Crc = Crc & 0x80 ? (Crc << 1) ^ 0xEB : Crc << 1;
        Crc = Crc & 0x80 ? (Crc << 1) ^ 0xEB : Crc << 1;
        Crc = Crc & 0x80 ? (Crc << 1) ^ 0xEB : Crc << 1;
    }

    return Crc;
}

/**
 * @file ht_lib_internal.h
 */
int16 HT_PROTOCOL_DecimalToHexstr(void *Var, HT_PROTOCOL_CustomTypes_t DataType, char *Str) {

    /* Specific union for double casting to hexa */
    union {
        long long i; /* 8 bytes */
        double    d; /* 8 bytes */
    } DoubleValue;

    /* Specific union for float casting to hexa */
    union {
        uint32 i; /* 4 bytes */
        float  d; /* 4 bytes */
    } FloatValue;

    void *AuxPtr;
    uint8 BytesLength = 0;

    switch (DataType) {
        case INT8_DATA:
            AuxPtr = (int8 *) Var;
            /* This has to be converted to uint8 to be printed */
            sprintf(Str, "%02" PRIX8, *(uint8 *) AuxPtr);
            BytesLength = 1;
            break;
        case UINT8_DATA:
            AuxPtr = (uint8 *) Var;
            sprintf(Str, "%02" PRIX8, *(uint8 *) AuxPtr);
            BytesLength = 1;
            break;
        case INT16_DATA:
            AuxPtr = (int16 *) Var;
            sprintf(Str, "%04" PRIX16, *(uint16 *) AuxPtr);
            BytesLength = 2;
            break;
        case UINT16_DATA:
            AuxPtr = (uint16 *) Var;
            sprintf(Str, "%04" PRIX16, *(uint16 *) AuxPtr);
            BytesLength = 2;
            break;
        case INT32_DATA:
            AuxPtr = (int32 *) Var;
            sprintf(Str, "%08" PRIX32, *(uint32 *) AuxPtr);
            BytesLength = 4;
            break;
        case UINT32_DATA:
            AuxPtr = (uint32 *) Var;
            sprintf(Str, "%08" PRIX32, *(uint32 *) AuxPtr);
            BytesLength = 4;
            break;
        case INT64_DATA:
            AuxPtr = (int64 *) Var;
            sprintf(Str, "%016" PRIX64, *(uint64 *) AuxPtr);
            BytesLength = 8;
            break;
        case UINT64_DATA:
            AuxPtr = (uint64 *) Var;
            sprintf(Str, "%016" PRIX64, *(uint64 *) AuxPtr);
            BytesLength = 8;
            break;
        case FLOAT_DATA:
            AuxPtr       = (float *) Var;
            FloatValue.d = *(float *) AuxPtr;
            sprintf(Str, "%08" PRIX32, FloatValue.i);
            BytesLength = 4;
            break;
        case DOUBLE_DATA:
            AuxPtr        = (double *) Var;
            DoubleValue.d = *(double *) AuxPtr;
            sprintf(Str, "%016llX", DoubleValue.i);
            BytesLength = 8;
            break;
        default:
            return HT_ERROR_INVALID_DATA_TYPE;
    }

    return BytesLength * 2;
}

/**
 * @file ht_lib_internal.h
 */
bool HT_PROTOCOL_IsMsgHt(void *Msg) {

    if (Msg == NULL) {
        return false;
    }
    char *Ptr = (char *) Msg;

    return Ptr[0] == HT_HW_START_CHAR;
}

/**
 * @file ht_lib_internal.h
 */
size_t HT_PROTOCOL_DecBytes(void *DecodedMsg, void *Msg, size_t NumOfBytes) {

    if (Msg == NULL) {
        return HT_ERROR_NULL_POINTER;
    }

    if (DecodedMsg == NULL) {
        return HT_ERROR_NULL_POINTER;
    }

    char *dPtr  = (char *) Msg;
    int   Index = 0;

    for (size_t i = 0; i < NumOfBytes; i += 2) {
        /* Not handling error case for hard coded value */
        Index += HT_PROTOCOL_HexstrConverter((char *) DecodedMsg + Index, UINT8_DATA, dPtr, i);
    }

    return Index;
}

/**
 * @file ht_lib_internal.h
 */
size_t HT_PROTOCOL_EncBytes(char *EncodedMsg, void *Data, size_t NumOfBytes) {

    if (EncodedMsg == NULL || Data == NULL) {
        return 0;
    }

    char *DataPtr = (char *) Data;
    int   Index   = 0;

    for (size_t i = 0; i < NumOfBytes; i++) {
        /* Not handling error case for hard coded value */
        Index += HT_PROTOCOL_DecimalToHexstr(DataPtr + i, UINT8_DATA, EncodedMsg + Index);
    }

    return Index;
}

/**
 * @file ht_lib.h
 */
int32 HT_PROTOCOL_InitMsg(HT_PROTOCOL_Msg_t *Msg, uint8 SrcId, uint8 DestId, uint8 CmdId, uint16 PayloadSize, void *Payload, bool UseCrc) {

    if (Msg == NULL) {
        return HT_ERROR_NULL_POINTER;
    }

    Msg->Header.SrcId      = SrcId;
    Msg->Header.DestId     = DestId;
    Msg->Header.CmdId      = CmdId;
    Msg->Header.DataLength = PayloadSize;
    Msg->Data = Payload;
    Msg->UseCrc = UseCrc;
    Msg->CrcValue = 0;

    return HT_STATUS_SUCCESS;
}

/**
 * @file ht_lib.h
 */
int32 HT_PROTOCOL_EncodeMsg(char *EncodedMsg, HT_PROTOCOL_Msg_t *Msg) {

    if (Msg == NULL) {
        return HT_ERROR_NULL_POINTER;
    }

    uint16 Index   = 0;
    char  *Encoded = EncodedMsg;

    Index += sprintf(Encoded, "%c", HT_HW_START_CHAR);
    /* Not handling error case for hard coded value */
    Index += HT_PROTOCOL_DecimalToHexstr((void *) &Msg->Header.SrcId, UINT8_DATA, Encoded + Index);
    Index += HT_PROTOCOL_DecimalToHexstr((void *) &Msg->Header.DestId, UINT8_DATA, Encoded + Index);
    Index += HT_PROTOCOL_DecimalToHexstr((void *) &Msg->Header.CmdId, UINT8_DATA, Encoded + Index);
    Index += HT_PROTOCOL_DecimalToHexstr((void *) &Msg->Header.DataLength, UINT16_DATA, Encoded + Index);

    Index += HT_PROTOCOL_EncBytes(Encoded + Index, Msg->Data, Msg->Header.DataLength);

    if (Msg->UseCrc) {
        Index += sprintf(&Encoded[Index], "%c", HT_CRC_CHAR);
        /* Not handling error case for hard coded value */
        Msg->CrcValue = HT_PROTOCOL_Crc8(Encoded + 1,  Index - 1);
        Index += HT_PROTOCOL_DecimalToHexstr((void *) &Msg->CrcValue, UINT8_DATA, Encoded + Index);
    }

    Index += sprintf(&Encoded[Index], "%c", HT_HW_END_CHAR);

    return Index;
}

/*
 * @file ht_lib.h
 */
int32 HT_PROTOCOL_DecodeMsg(HT_PROTOCOL_Msg_t *DecodedMsg, void *DecodedPayload, size_t DecodedPayloadSize, char *EncodedMsg) {

    size_t Index = 0;

    if (EncodedMsg == NULL || DecodedPayload == NULL || DecodedMsg == NULL) {
        return HT_ERROR_NULL_POINTER;
    }

    if (!HT_PROTOCOL_IsMsgHt(EncodedMsg)) {
        return HT_ERROR_NOT_HT_MSG;
    }

    Index = 1;

    /* Not handling error case for hard coded value */
    Index += 2 * HT_PROTOCOL_HexstrConverter(&DecodedMsg->Header.SrcId, UINT8_DATA, EncodedMsg + Index, 0);
    Index += 2 * HT_PROTOCOL_HexstrConverter(&DecodedMsg->Header.DestId, UINT8_DATA, EncodedMsg + Index, 0);
    Index += 2 * HT_PROTOCOL_HexstrConverter(&DecodedMsg->Header.CmdId, UINT8_DATA, EncodedMsg + Index, 0);
    Index += 2 * HT_PROTOCOL_HexstrConverter(&DecodedMsg->Header.DataLength, UINT16_DATA, EncodedMsg + Index, 0);

    /* Not enough bytes were allocated */
    if (DecodedPayloadSize < DecodedMsg->Header.DataLength) {
        return HT_ERROR_WRONG_MSG_LENGTH;
    }

    DecodedMsg->Data = DecodedPayload;
    Index += 2 * HT_PROTOCOL_DecBytes(DecodedMsg->Data, EncodedMsg + Index, DecodedMsg->Header.DataLength * 2);
    DecodedMsg->UseCrc = (EncodedMsg[Index] == HT_CRC_CHAR);

    if (DecodedMsg->UseCrc) {
        Index += 1;
        /* Ignore start byte ('$'), end byte and 2 bytes for encoded CRC */
        uint8 CalculatedCrc = HT_PROTOCOL_Crc8(EncodedMsg + 1, HT_PROTOCOL_CALC_ENCODED_SIZE(DecodedMsg->Header.DataLength) - 4);
        /* Not handling error case for hard coded value */
        Index += 2 * HT_PROTOCOL_HexstrConverter(&DecodedMsg->CrcValue, UINT8_DATA, EncodedMsg + Index, 0);
        if (DecodedMsg->CrcValue != CalculatedCrc) {
            return HT_ERROR_CRC;
        }
    }

    if (EncodedMsg[Index] != HT_HW_END_CHAR) {
        return HT_ERROR_WRONG_END_CHAR;
    }

    return Index;
}

/**
 * @file ht_lib.h
 */
int32 HT_PROTOCOL_DecodePayload(void *DecodedPayload, size_t DecodedPayloadSize, char *EncodedMsg) {

    size_t Index = 0;

    if (EncodedMsg == NULL || DecodedPayload == NULL) {
        return HT_ERROR_NULL_POINTER;
    }

    if (!HT_PROTOCOL_IsMsgHt(EncodedMsg)) {
        return HT_ERROR_NOT_HT_MSG;
    }

    if ((size_t) HT_PROTOCOL_GetMsgLength(EncodedMsg) > DecodedPayloadSize) {
        return HT_ERROR_WRONG_MSG_LENGTH;
    }

    HT_PROTOCOL_Msg_t DecodedMsg;
    Index = HT_PROTOCOL_DecodeMsg(&DecodedMsg, DecodedPayload, DecodedPayloadSize, EncodedMsg);

    return Index;
}

