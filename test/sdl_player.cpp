#include "sdl_player.h"
#include "video_frame/video_frame_buffer.h"
#include "video_frame/i420_buffer.h"

void SdlPlayer::start(int width, int height)
{
	_width = width;
	_height = height;
	_is_stop = false;
	if (!_th)
	{
		_th = std::make_shared<std::thread>(&SdlPlayer::player_thread, this);
	}
}

void SdlPlayer::stop()
{
	_is_stop = true;
	if (_th)
	{
		_th->join();
		_th = nullptr;
	}
}

void SdlPlayer::OnFrame(const webrtc::VideoFrame& frame)
{
	_qu.push_back(frame);
}

void SdlPlayer::player_thread()
{
	  SDL_Rect        rect;
	  Uint32 	  pixformat; 

	  //for render
	  SDL_Window 	  *win = NULL;
	  SDL_Renderer    *renderer = NULL;
	  SDL_Texture     *texture = NULL;
	  bool flag = false;

	  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
		//fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL - %s\n", SDL_GetError());
		return;
	  }

	  win = SDL_CreateWindow( "Media Player",
					  SDL_WINDOWPOS_UNDEFINED,
		  		  SDL_WINDOWPOS_UNDEFINED,
				  _width, _height,
				  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);	  
	  if(!win){
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window by SDL");  
			goto __QUIT;
	  }

	  renderer = SDL_CreateRenderer(win, -1, 0);
	  if(!renderer){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create Renderer by SDL");  
		goto __QUIT;
	  }

	  pixformat = SDL_PIXELFORMAT_IYUV;
	  texture = SDL_CreateTexture(renderer,
				pixformat, 
				SDL_TEXTUREACCESS_STREAMING,
				_width, 
				_height);

	  
	  while (!_is_stop) 
	  {

		  webrtc::VideoFrame frame = _qu.pop(flag, std::chrono::milliseconds(100));
		  if (flag)
		  {
			  SDL_UpdateYUVTexture(texture, NULL,
				  frame.video_frame_buffer()->GetI420()->DataY(), frame.video_frame_buffer()->GetI420()->StrideY(),
				  frame.video_frame_buffer()->GetI420()->DataU(), frame.video_frame_buffer()->GetI420()->StrideU(),
				  frame.video_frame_buffer()->GetI420()->DataV(), frame.video_frame_buffer()->GetI420()->StrideV());

			  // Set Size of Window
			  rect.x = 0;
			  rect.y = 0;
			  rect.w = _width;
			  rect.h = _height;

			  SDL_RenderClear(renderer);
			  SDL_RenderCopy(renderer, texture, NULL, &rect);
			  SDL_RenderPresent(renderer);
			  //SDL_Delay(41);

		  }
	  }

		  
	SDL_Event event;
	SDL_PollEvent(&event);
	switch(event.type) {
	case SDL_QUIT:
		goto __QUIT;
			break;
	default:
			break;
	}

__QUIT:
	if(win){
		SDL_DestroyWindow(win);
	}
	if(renderer){
		SDL_DestroyRenderer(renderer);
	}
	if(texture){
		SDL_DestroyTexture(texture);
	}
	SDL_Quit();
	return;
}
