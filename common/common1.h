#ifndef __COMMON1_H__
#define __COMMON1_H__

#include "sam.h"
#include <opencv2/opencv.hpp> 

bool load_and_precompute_image_from_file(std::string path, sam_image_u8 & img0, sam_state & state, int n_threads);
bool load_image_samformat_from_file(const std::string & fname, sam_image_u8 & img);
cv::Mat get_best_opencv_mask_at_point(int x, int y, sam_image_u8 img0, sam_state & state, int n_threads);

#endif
