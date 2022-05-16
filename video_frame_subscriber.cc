/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "video_frame_subscriber.h"

namespace webrtc {
namespace test {

	int VideoFrameSubscriber::global_subs_id = 0;

	VideoFrameSubscriber::VideoFrameSubscriber()
		:_subs_id(++global_subs_id)
	{
	}

	int VideoFrameSubscriber::SubsId() const
	{
		return _subs_id;
	}
}  // namespace test
}  // namespace webrtc
