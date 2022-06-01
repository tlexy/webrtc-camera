#include "x264_encoder.h"
#include "video_frame/video_frame_buffer.h"
#include "video_frame/i420_buffer.h"

X264Encoder::X264Encoder()
	:webrtc::test::VideoFrameSubscriber()
{}

void X264Encoder::OnFrame(const webrtc::VideoFrame& frame)
{
	_qu.push_back(frame);
}

void X264Encoder::init(int width, int height, int fps)
{
	_width = width;
	_height = height;
	_fps = fps;

	/*x264_param_t mParam;
	x264_param_default(&mParam);*/

	int iNal = 0;
	x264_nal_t* pNals = NULL;
	x264_t* pHandle = NULL;
	x264_param_t* pParam = (x264_param_t*)malloc(sizeof(x264_param_t));
	x264_param_default(pParam);   //�������ṹ�帳Ĭ��ֵ
	//����preset��tune
	x264_param_default_preset(pParam, "fast", "zerolatency");
	pParam->i_csp = X264_CSP_I420;//yuv420p
	pParam->i_width = width;   // ���
	pParam->i_height = height;  // �߶�
	pParam->i_fps_num = 25;     // ����֡�ʣ����ӣ�
	pParam->i_fps_den = 1;      // ����֡��ʱ��1s����ĸ��

	pParam->i_threads = X264_SYNC_LOOKAHEAD_AUTO;
	pParam->i_keyint_max = 10;              //�ڴ˼������IDR�ؼ�֡
	///slice :live ֱ��
	pParam->i_slice_count = 4;

	pParam->rc.i_bitrate = 1200;       // ��������,��ABR(ƽ������)ģʽ�²���Ч���ұ���������ABRǰ������bitrate
	pParam->rc.i_rc_method = X264_RC_ABR;  // ���ʿ��Ʒ�����CQP(�㶨����)��CRF(�㶨����,ȱʡֵ23)��ABR(ƽ������)
	x264_param_apply_profile(pParam, "baseline");

	//open encoder
	pHandle = x264_encoder_open(pParam);
	_handle = pHandle;
	_param = pParam;
}

void X264Encoder::start()
{
	_is_stop = false;
	if (!_th)
	{
		_th = std::make_shared<std::thread>(&X264Encoder::encode_thread, this);
	}
}

void X264Encoder::stop()
{
	_is_stop = true;
	if (_th)
	{
		_th->join();
		_th = nullptr;
	}
}

void X264Encoder::encode_thread()
{
	x264_picture_t* pPic_in = (x264_picture_t*)malloc(sizeof(x264_picture_t));
	x264_picture_t* pPic_out = (x264_picture_t*)malloc(sizeof(x264_picture_t));
	x264_picture_init(pPic_out);//out.picture
	//I420,yuv420p
	x264_picture_alloc(pPic_in, X264_CSP_I420, _param->i_width, _param->i_height);
	bool flag = false;
	int iNal = 0;
	x264_nal_t* pNals = NULL;
	int ret = 0;
	while (!_is_stop) 
	{
		webrtc::VideoFrame frame = _qu.pop(flag, std::chrono::milliseconds(100));
		if (flag)
		{
			memcpy(pPic_in->img.plane[0], frame.video_frame_buffer()->GetI420()->DataY(), frame.video_frame_buffer()->GetI420()->StrideY());
			memcpy(pPic_in->img.plane[1], frame.video_frame_buffer()->GetI420()->DataU(), frame.video_frame_buffer()->GetI420()->StrideU());
			memcpy(pPic_in->img.plane[2], frame.video_frame_buffer()->GetI420()->DataV(), frame.video_frame_buffer()->GetI420()->StrideV());
			ret = x264_encoder_encode(_handle, &pNals, &iNal, pPic_in, pPic_out);
			if (ret < 0) {
				printf("Error.\n");
				break;
			}
		}
	}
}
