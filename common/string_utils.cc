/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "string_utils.h"

namespace string_utils {

  const char kToLower[256] = {
  '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
  '\x08', '\x09', '\x0a', '\x0b', '\x0c', '\x0d', '\x0e', '\x0f',
  '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
  '\x18', '\x19', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f',
  '\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27',
  '\x28', '\x29', '\x2a', '\x2b', '\x2c', '\x2d', '\x2e', '\x2f',
  '\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37',
  '\x38', '\x39', '\x3a', '\x3b', '\x3c', '\x3d', '\x3e', '\x3f',
  '\x40',    'a',    'b',    'c',    'd',    'e',    'f',    'g',
     'h',    'i',    'j',    'k',    'l',    'm',    'n',    'o',
     'p',    'q',    'r',    's',    't',    'u',    'v',    'w',
     'x',    'y',    'z', '\x5b', '\x5c', '\x5d', '\x5e', '\x5f',
  '\x60', '\x61', '\x62', '\x63', '\x64', '\x65', '\x66', '\x67',
  '\x68', '\x69', '\x6a', '\x6b', '\x6c', '\x6d', '\x6e', '\x6f',
  '\x70', '\x71', '\x72', '\x73', '\x74', '\x75', '\x76', '\x77',
  '\x78', '\x79', '\x7a', '\x7b', '\x7c', '\x7d', '\x7e', '\x7f',
  '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87',
  '\x88', '\x89', '\x8a', '\x8b', '\x8c', '\x8d', '\x8e', '\x8f',
  '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97',
  '\x98', '\x99', '\x9a', '\x9b', '\x9c', '\x9d', '\x9e', '\x9f',
  '\xa0', '\xa1', '\xa2', '\xa3', '\xa4', '\xa5', '\xa6', '\xa7',
  '\xa8', '\xa9', '\xaa', '\xab', '\xac', '\xad', '\xae', '\xaf',
  '\xb0', '\xb1', '\xb2', '\xb3', '\xb4', '\xb5', '\xb6', '\xb7',
  '\xb8', '\xb9', '\xba', '\xbb', '\xbc', '\xbd', '\xbe', '\xbf',
  '\xc0', '\xc1', '\xc2', '\xc3', '\xc4', '\xc5', '\xc6', '\xc7',
  '\xc8', '\xc9', '\xca', '\xcb', '\xcc', '\xcd', '\xce', '\xcf',
  '\xd0', '\xd1', '\xd2', '\xd3', '\xd4', '\xd5', '\xd6', '\xd7',
  '\xd8', '\xd9', '\xda', '\xdb', '\xdc', '\xdd', '\xde', '\xdf',
  '\xe0', '\xe1', '\xe2', '\xe3', '\xe4', '\xe5', '\xe6', '\xe7',
  '\xe8', '\xe9', '\xea', '\xeb', '\xec', '\xed', '\xee', '\xef',
  '\xf0', '\xf1', '\xf2', '\xf3', '\xf4', '\xf5', '\xf6', '\xf7',
  '\xf8', '\xf9', '\xfa', '\xfb', '\xfc', '\xfd', '\xfe', '\xff',
    };
    
  const size_t SIZE_UNKNOWN = static_cast<size_t>(-1);

    inline char ascii_tolower(unsigned char c) {
        return kToLower[c];
    }

    int memcasecmp(const char* s1, const char* s2, size_t len) {
        const unsigned char* us1 = reinterpret_cast<const unsigned char*>(s1);
        const unsigned char* us2 = reinterpret_cast<const unsigned char*>(s2);

        for (size_t i = 0; i < len; i++) {
            const int diff =
                int{ static_cast<unsigned char>(ascii_tolower(us1[i])) } -
                int{ static_cast<unsigned char>(ascii_tolower(us2[i])) };
            if (diff != 0) return diff;
        }
        return 0;
    }

    bool EqualsIgnoreCase(const std::string& piece1, const std::string& piece2) {
        return (piece1.size() == piece2.size() &&
            0 == memcasecmp(piece1.data(), piece2.data(),
                piece1.size()));
    }

    size_t strcpyn(char* buffer,
        size_t buflen,
        const char* source,
        size_t srclen /* = SIZE_UNKNOWN */) {
        if (buflen <= 0)
            return 0;

        if (srclen == SIZE_UNKNOWN) {
            srclen = strlen(source);
        }
        if (srclen >= buflen) {
            srclen = buflen - 1;
        }
        memcpy(buffer, source, srclen);
        buffer[srclen] = 0;
        return srclen;
    }

    static const char kWhitespace[] = " \n\r\t";

    std::string string_trim(const std::string& s) {
        std::string::size_type first = s.find_first_not_of(kWhitespace);
        std::string::size_type last = s.find_last_not_of(kWhitespace);

        if (first == std::string::npos || last == std::string::npos) {
            return std::string("");
        }

        return s.substr(first, last - first + 1);
    }

    std::string ToHex(const int i) {
        char buffer[50];
        snprintf(buffer, sizeof(buffer), "%x", i);

        return std::string(buffer);
    }

} 
