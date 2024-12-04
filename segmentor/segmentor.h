#ifndef __SEGMENTOR_H__
#define __SEGMENTOR_H__

#include "stdio.h"
#include <opencv2/core.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
//#include <iostream>
//#include <opencv2/video/video.hpp>
  
class Segmentor {      
  public:   

  	virtual void test() = 0; //= 0 means "pure virtual"
    virtual bool preprocessImage(cv::Mat& image) = 0;

    Segmentor() {
      printf("Called Segmentor constructor\n");
    }
};

#endif
