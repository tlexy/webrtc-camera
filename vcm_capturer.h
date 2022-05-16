/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#ifndef TEST_VCM_CAPTURER_H_
#define TEST_VCM_CAPTURER_H_

#include <memory>
#include <vector>

#include "common/scoped_refptr.h"
#include "core/video_capture.h"
#include "video_frame/video_frame.h"
#include "video_frame/video_sink_interface.h"
#include "common/threadqueue.hpp"
#include <thread>
#include <memory>
#include <vector>

#pragma execution_character_set("utf-8")

namespace webrtc {
namespace test {

class VideoFrameSubscriber;

class VcmCapturer : public rtc::VideoSinkInterface<VideoFrame> {
 public:
  static VcmCapturer* Create(size_t width,
                             size_t height,
                             size_t target_fps,
                             size_t capture_device_index);
  virtual ~VcmCapturer();

  bool StartCapture();
  void AddSubscriber(std::shared_ptr<VideoFrameSubscriber>);
  void DelSubscriber(std::shared_ptr<VideoFrameSubscriber>);

  void OnFrame(const VideoFrame& frame) override;

  int RealWidth();
  int RealHeight();

 private:
  VcmCapturer();
  bool Init(size_t width,
            size_t height,
            size_t target_fps,
            size_t capture_device_index);
  void Destroy();

  void broadcaster_thread();

  rtc::scoped_refptr<VideoCaptureModule> vcm_;
  VideoCaptureCapability capability_;
  ThreadQueue<VideoFrame> _qu;
  std::shared_ptr<std::thread> _th{nullptr};
  bool _is_stop{ true };
  /////////////////////NOTICE: 这里对_subs的操作并没有加锁
  std::vector<std::shared_ptr<VideoFrameSubscriber>> _subs;
  webrtc::VideoCaptureCapability _real_cap;
};

}  // namespace test
}  // namespace webrtc

#endif  // TEST_VCM_CAPTURER_H_
