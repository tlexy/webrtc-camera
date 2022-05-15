/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef API_STRING_UTIL_H_
#define API_STRING_UTIL_H_

#include <string>

namespace string_utils {

	int memcasecmp(const char* s1, const char* s2, size_t len);
	bool EqualsIgnoreCase(const std::string& piece1, const std::string& piece2);

	size_t strcpyn(char* buffer,
		size_t buflen,
		const char* source,
		size_t srclen /* = SIZE_UNKNOWN */);

	std::string string_trim(const std::string& s);
	std::string ToHex(const int i);
} 

#endif  
