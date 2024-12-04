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
    bool preprocessImage(cv::Mat& image);         
    SAM1Segmentor();
};
  
Segmentor& get_sam1_segmentor();

#endif
