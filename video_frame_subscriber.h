/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#ifndef TEST_VCM_VIDEO_FRAME_SUBSCRIBER_H_
#define TEST_VCM_VIDEO_FRAME_SUBSCRIBER_H_

#include "video_frame/video_frame.h"


#pragma execution_character_set("utf-8")

namespace webrtc {
	namespace test {

		class VideoFrameSubscriber
		{
		public:
			VideoFrameSubscriber();
			int SubsId() const;
			virtual void OnFrame(const VideoFrame&) = 0;

		private:
			static int global_subs_id;
			int _subs_id{0};
		};

	}
}

#endif  
