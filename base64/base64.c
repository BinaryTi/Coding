/**
 * @file        base64.c
 * @brief       base64
 * @author      gtWu
 * @version     v1.0
 * @date        2018-08-06
 */

#include <stdio.h>
#include <error.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"

/**
 * @brief       base64 encode
 *
 * @param       in          unencode buffer
 * @param       inSize      unencode buffer size
 * @param       out         encode buffer
 * @param       outSize     encode buffer size
 *
 * @return     0 or minEncodeBufferSize
 */
size_t Base64Encode(const unsigned char* in, size_t inSize,
                    unsigned char* out, size_t outSize)
{
    size_t minOutSize = (inSize % 3 == 0) ? (inSize / 3) * 4 : ((inSize + 3) / 3) * 4;
    
    // Get out buffer size
    if(out == NULL || outSize == 0)
        return minOutSize;
    // outSize > 0, but size is not enouth
    if(outSize < minOutSize)
    {
        perror("Out buffer size is not enough\n!");
        return 0;
    }
    
    unsigned char lastBlock[3] = { 0x00 };
    const unsigned char* inDataBlock = in;
    unsigned char * outDataBlock = out;
    size_t dataBlockNum = 0;
    uint8_t paddingNum = 0;

    for(dataBlockNum = 0; dataBlockNum * 3 < inSize; ++dataBlockNum)
    {
        // get padding size
        if((dataBlockNum + 1) * 3 == inSize + 1)
        {
            lastBlock[0] = in[dataBlockNum * 3];
            lastBlock[1] = in[dataBlockNum * 3 + 1];
            paddingNum = 1;
            inDataBlock = lastBlock;
        }
        else if((dataBlockNum + 1) * 3 == inSize + 2)
        {
            lastBlock[0] = in[dataBlockNum * 3];
            paddingNum = 2;
            inDataBlock = lastBlock;
        }
        else
            inDataBlock = in + dataBlockNum * 3;    /* end if */
        
        // 3 bytes => 4 bytes
        outDataBlock[0 + dataBlockNum * 4] = inDataBlock[0] >> 2;
        outDataBlock[1 + dataBlockNum * 4] = (inDataBlock[0] & 0x03) << 4 | inDataBlock[1] >> 4;
        outDataBlock[2 + dataBlockNum * 4] = (inDataBlock[1] & 0x0f) << 2 | inDataBlock[2] >> 6;
        outDataBlock[3 + dataBlockNum * 4] = (inDataBlock[2] & 0x3f);

        // replace character with base64code
        outDataBlock[0 + dataBlockNum * 4] = base64_table[outDataBlock[0 + dataBlockNum * 4]];
        outDataBlock[1 + dataBlockNum * 4] = base64_table[outDataBlock[1 + dataBlockNum * 4]];
        if(paddingNum >= 2)
            outDataBlock[2 + dataBlockNum * 4] = '=';
        else    
            outDataBlock[2 + dataBlockNum * 4] = base64_table[outDataBlock[2 + dataBlockNum * 4]];
        if(paddingNum >= 1)
            outDataBlock[3 + dataBlockNum * 4] = '=';
        else
            outDataBlock[3 + dataBlockNum * 4] = base64_table[outDataBlock[3 + dataBlockNum * 4]];
    }
    return minOutSize;
}

/**
 * @brief       return the index of base64char
 *
 * @param       ch
 *
 * @return      index of base64char(ch)
 */
unsigned char GetBase64CharValue(const unsigned char ch)
{
    if(ch >= 'A' && ch <= 'Z')
        return 0 + (ch - 'A');
    else if(ch >= 'a' && ch <= 'z')
        return 26 + (ch - 'a');
    else if(ch >= '0' && ch <= '9')
        return 52 + (ch - '0');
    else if(ch == '+')
        return 62;
    else if(ch == '/')
        return 63;
    else if(ch == '=' || ch == 0x00)
        return 0;
    else
    {
        perror("Invalid base64 character");
        return 0xff;
    }
}

/**
 * @brief       base64 decode
 *
 * @param       in          undecode buffer
 * @param       inSize      undecode buffer size
 * @param       out         decode buffer
 * @param       outSize     decode buffer size
 * @return      0 or minDecodeBufferSize
 */
size_t Base64Decode(const unsigned char* in, size_t inSize,
                    unsigned char* out, size_t outSize)
{
    size_t minOutSize = (inSize % 4 == 0) ? (inSize / 4) * 3 : ((inSize + 4) / 4) * 3;
    // Get out buffer size
    if(out == NULL || outSize == 0)
        return minOutSize;
    // outSize > 0, but size is not enouth
    if(outSize < minOutSize)
    {
        perror("Out buffer size is not enough\n!");
        return 0;
    }
    size_t dataBlockNum = 0;
    size_t index = 0;
    unsigned char value;
    unsigned char *inData = (unsigned char*)malloc(inSize);
    unsigned char* inDataBlock = inData;
    unsigned char* outDataBlock = out;
    for(index = 0; index < inSize; ++index)
    {
        value = GetBase64CharValue(in[index]);
        if(value == 0xff)
            return 0;
        inData[index] = value;
    }

    for(dataBlockNum = 0; dataBlockNum * 4 < inSize; ++dataBlockNum)
    {
        inDataBlock = inData + dataBlockNum * 4;
        outDataBlock = out + dataBlockNum * 3;
        outDataBlock[0] = (inDataBlock[0] << 2 & 0xfc) | (inDataBlock[1] >> 4 & 0x03);
        if(dataBlockNum * 4 + 2 < inSize)    // unused '=' to padding
            outDataBlock[1] = (inDataBlock[1] << 4 & 0xf0) | (inDataBlock[2] >> 2 & 0x0f);
        if(dataBlockNum * 4 + 3 < inSize)
            outDataBlock[2] = (inDataBlock[2] << 6 & 0xc0) | (inDataBlock[3] & 0x3f);
    }
    
    return minOutSize;
}
