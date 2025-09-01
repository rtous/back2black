#ifndef __SAM21FACTORY_H__
#define __SAM21FACTORY_H__

#include "segmentor.h" 
#include <opencv2/core.hpp>
#include "onnxruntime_utils.h" 
#include "sam21.h"
//#include "sam2_1.h"
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
//#include <iostream>
//#include <opencv2/video/video.hpp>

class SAM21Segmentor : public Segmentor {      
  public: 
    SAM21 sam21; 
    //Sam sam;
    //sam_params params;
    //std::shared_ptr<sam_state> a_sam_state;

    
    void test();  
    bool preprocessImage(cv::Mat& image_opencv);  
    bool preprocessImage(sam_image_u8& image_sam);
    bool preprocessImage_and_remember(cv::Mat& image);//assumes frames are preprocessed in order
    bool preprocessImage_and_remember(sam_image_u8& image_sam);//assumes frames are preprocessed in order
    //es pot eliminar:
    cv::Mat get_best_mask_at_point(int x, int y, cv::Mat& image_opencv);
    cv::Mat get_best_mask_at_points(std::vector<MaskPoint>& points, cv::Mat& image_opencv);
    //es pot eliminar:
    bool get_best_mask_at_point(int x, int y, sam_image_u8& image_sam, sam_image_u8& mask_sam);
    bool get_best_mask_at_points(std::vector<MaskPoint>& points, sam_image_u8& image_sam, sam_image_u8& mask_sam);
    bool get_best_mask_at_points_video(std::vector<MaskPoint>& points, sam_image_u8& image_sam, sam_image_u8& mask_sam, int frame_idx);
    void reset_memory();
    SAM21Segmentor();
    void close();
};
  
Segmentor& get_sam21_segmentor();

#endif
