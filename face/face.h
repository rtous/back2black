#ifndef __FACE_H__
#define __FACE_H__

#include <opencv2/opencv.hpp> 

void face(cv::Mat img, cv::Mat & res_face, cv::Mat & res_eyes, cv::Scalar color, cv::Scalar pupilsColor);

#endif
