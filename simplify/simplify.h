#ifndef __SIMPLIFY_H__
#define __SIMPLIFY_H__

#include <stdio.h>
#include <opencv2/opencv.hpp> 
#include <opencv2/core/utils/filesystem.hpp>

void simplify(cv::Mat &input_image, cv::Mat &output_image);
std::vector<std::vector<cv::Point>> simplifyColorSegment(cv::Mat &mask, cv::Mat &output_image, bool randomColor, int R, int G, int B); 
void fillContoursWithColorAndAlpha(std::vector<std::vector<cv::Point>> contours, cv::Mat &output_image, bool randomColor, int R, int G, int B);


#endif
