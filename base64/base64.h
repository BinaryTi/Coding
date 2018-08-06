/**
 * @file        base64.h
 * @brief       base64
 * @author      gtWu
 * @version     v1.0
 * @date        2018-08-06
 */

#ifndef _BASE_64_H_
#define _BASE_64_H_

#include <stddef.h>

const char base64_table[] = 
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

size_t Base64Encode(const unsigned char* in, size_t in_size,
                    unsigned char* out, size_t out_size);
size_t Base64Decode(const unsigned char* in, size_t in_size,
                    unsigned char* out, size_t out_size);
#endif
