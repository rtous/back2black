#ifndef __SEGMENTOR_H__
#define __SEGMENTOR_H__

#include "stdio.h"
#include <opencv2/core.hpp>
#include "sam.h"
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
//#include <iostream>
//#include <opencv2/video/video.hpp>

/*class MaskPoint {      
  public:             
    int x;
    int y;
    bool positive;
};
//Ho he possat a sam.h per que pugui ser accedir per data_structures.h
*/
  
class Segmentor {      
  public:   

    //WARNING: Do not forget "= 0"

  	virtual void test() = 0; //= 0 means "pure virtual"
    virtual bool preprocessImage(cv::Mat& image) = 0;
    virtual bool preprocessImage(sam_image_u8& image_sam) = 0;
    //es pot eliminar:
    virtual cv::Mat get_best_mask_at_point(int x, int y, cv::Mat& image_opencv) = 0;
    //es pot eliminar:
    virtual bool get_best_mask_at_point(int x, int y, sam_image_u8& image_sam, sam_image_u8& mask_sam) = 0;
    virtual bool get_best_mask_at_points(std::vector<MaskPoint>& points, sam_image_u8& image_sam, sam_image_u8& mask_sam) = 0;
    virtual void close() = 0;
    

    Segmentor() {
      printf("Called Segmentor constructor\n");
    }
};

#endif
