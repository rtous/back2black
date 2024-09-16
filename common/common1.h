#ifndef __COMMON1_H__
#define __COMMON1_H__

#include "sam.h"
#include "data_structures.h"
#include <opencv2/opencv.hpp> 

bool load_and_precompute_image_from_file(std::string path, sam_image_u8 & img0, sam_state & state, int n_threads);
bool load_image_samformat_from_file(const std::string & fname, sam_image_u8 & img);
void get_best_sam_mask_at_point(int x, int y, sam_image_u8 img0, sam_state & state, int n_threads, sam_image_u8 & mask);
cv::Mat get_best_opencv_mask_at_point(int x, int y, sam_image_u8 img0, sam_state & state, int n_threads);
void compute_object(Object & anObject, sam_image_u8 img0, sam_state & state, int n_threads);
int propagate_masks(std::vector<Frame> & frames, sam_state & state, int n_threads);
void sam_image2opencv(sam_image_u8 & sam_image, cv::Mat & opencv_image);
void opencv_image2sam(sam_image_u8 & sam_image, cv::Mat & opencv_image);


//void example_func();

#endif
