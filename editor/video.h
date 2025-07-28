#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <string>
#include <opencv2/opencv.hpp> 
#include "data_structures.h"
#include "imgui_impl_sdl2.h"
#include "state.h"

void read_video(const std::string &videoFilePath, Video &theVideo, int w, int h);
void save_video(MyState &myState, const std::string &videoFilePath, Video &theVideo);
void save_frame(const std::string &frameFilePath, Video &theVideo, int selected_frame);
void save_video_frames(MyState &myState, const std::string &videoFilePath, Video &theVideo);

#endif
