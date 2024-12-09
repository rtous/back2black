#ifndef __COMMON1_H__
#define __COMMON1_H__

#include "sam.h"
#include "data_structures.h"
#include <opencv2/opencv.hpp> 

bool load_and_precompute_image_from_file(std::string path, sam_image_u8 & img0, sam_state & state, int n_threads);
bool load_image_samformat_from_file(const std::string & fname, sam_image_u8 & img);
bool get_best_sam_mask_at_point(int x, int y, sam_image_u8 img0, sam_state & state, int n_threads, sam_image_u8 & mask);
cv::Mat get_best_opencv_mask_at_point(int x, int y, sam_image_u8 img0, sam_state & state, int n_threads);
//void compute_mask(Mask & aMask, sam_image_u8 img0, sam_state & state, int n_threads);
bool compute_mask(Mask & aMask, sam_image_u8 img0, sam_state & state, int n_threads);

void compute_mask_center(Mask & aMask);
//int propagate_masks(std::vector<Frame> & frames, sam_state & state, int n_threads);
int propagate_masks(std::vector<Frame> & frames, sam_state & state, int n_threads, int from_frame, int till_frame, float & progress, bool & cancel);
void sam_image_grayscale2opencv(sam_image_u8 & sam_image, cv::Mat & opencv_image);
void sam_image_color2opencv(sam_image_u8 & sam_image, cv::Mat & opencv_image);

void opencv_image2sam(sam_image_u8 & sam_image, cv::Mat & opencv_image); 
sam_image_u8 sam_mask_to_sam_4channels(sam_image_u8 & sam_image, int alpha);
void sam_image_grayscale2opencv_colorize(sam_image_u8 & sam_image, cv::Mat & opencv_image, int R, int G, int B);//WARNING: NOT USAGE DETECTED!!
void opencv_image2sam_binarymask(sam_image_u8 & sam_image, cv::Mat & opencv_image);
int load_frames_from_files(std::string input_path, std::vector<Frame> & frames);


//void example_func();

#endif
