#ifndef __OPENCVLIBRARYEXAMPLE_H__
#define __OPENCVLIBRARYEXAMPLE_H__

#include <opencv2/opencv.hpp> 
#include <opencv2/core/utils/filesystem.hpp>
#include <stdio.h>
#include "sam.h"

class APIState {
    public:  
        //std::shared_ptr<sam_state> state;
        std::shared_ptr<sam_state> state;
        sam_params params; 
        sam_image_u8 sam_image;
        std::string checkpoint_path;

        APIState();
        void set_checkpoint_path(std::string checkpoint_path_);
        void load_model();
        void precompute_image_opencv(cv::Mat input_img);
        void precompute_image(sam_image_u8 a_sam_image);
        std::vector<sam_image_u8> compute_masks_samformat(sam_point pt);
        std::vector<sam_image_u8> compute_masks_samformat(int x, int y);
        bool compute_mask_opencv(int x, int y, cv::Mat & output_img);
};

cv::Mat test();
#endif
