#include "ecc.h"

#include <algorithm>

using namespace ecc;


static const char *base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int base64_char(int c) {
    for (int i = 0; i < 64; i++) {
        if (base64[i] == c) return i;
    }
    return -1;
}

std::string ecc::base64_decode(std::string const &s) {
    unsigned in_len = s.size();
    int i = 0, j, in_ = 0, char_array_4[4], char_array_3[3];
    std::string result;

    while (in_len-- && ( s[in_] != '=') && base64_char(s[in_]) != -1) {
        char_array_4[i++] = (int) (unsigned char)s[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++) char_array_4[i] = base64_char(char_array_4[i]);
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            for (i = 0; (i < 3); i++) result.push_back((char) (unsigned char)char_array_3[i]);
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++) char_array_4[j] = 0;
        for (j = 0; j <4; j++) char_array_4[j] = base64_char(char_array_4[j]);
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        for (j = 0; (j < i - 1); j++) result.push_back((char) (unsigned char)char_array_3[j]);
    }

    return result;
}

std::string ecc::base64_encode(std::string const &s) {
    std::string result;
    int i = 0, j, char_array_3[3], char_array_4[4];
    for (char ch: s) {
        char_array_3[i++] = (int) (unsigned char) ch;
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            for (i = 0; (i < 4); i++) result += base64[char_array_4[i]];
            i = 0;
        }
    }
    if (i) {
        for (j = i; j < 3; j++) char_array_3[j] = '\0';
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        for (j = 0; (j < i + 1); j++) result += base64[char_array_4[j]];
        while ((i++ < 3)) result += '=';
    }
    return result;
}

