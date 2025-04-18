#ifndef __SAM1FACTORY_H__
#define __SAM1FACTORY_H__

#include "segmentor.h"
#include <opencv2/core.hpp>
#include "sam.h"
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
//#include <iostream>
//#include <opencv2/video/video.hpp>

class SAM1Segmentor : public Segmentor {      
  public:  
    sam_params params;
    std::shared_ptr<sam_state> a_sam_state;

    
    void test();  
    bool preprocessImage(cv::Mat& image_opencv);  
    bool preprocessImage(sam_image_u8& image_sam);
    cv::Mat get_best_mask_at_point(int x, int y, cv::Mat& image_opencv);
    cv::Mat get_best_mask_at_points(std::vector<MaskPoint> points, cv::Mat& image_opencv);
    bool get_best_mask_at_point(int x, int y, sam_image_u8& image_sam, sam_image_u8& mask_sam);
    bool get_best_mask_at_points(std::vector<MaskPoint>& points, sam_image_u8& image_sam, sam_image_u8& mask_sam);
    SAM1Segmentor();
    void close();
};
  
Segmentor& get_sam1_segmentor();

#endif
