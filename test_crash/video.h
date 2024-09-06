#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <string>
#include <opencv2/opencv.hpp> 
#include "data_structures.h"

void read_video(const std::string &videoFilePath, Video &theVideo);

#endif
