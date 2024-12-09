#include "segmentor_sam1.h"
#include "stdio.h"
#include <opencv2/imgcodecs.hpp>
#include "common1.h"

int main()
{

    ///////////////////////
    // test conversion utilities
    ///////////////////////
    std::string image_path = "/Users/rtous/dev/back2black/img.jpg";
    cv::Mat image_opencv = imread(image_path, cv::IMREAD_COLOR);

    //opencv -> sam
    sam_image_u8 image_sam;
    opencv_image2sam(image_sam, image_opencv);

    //sam -> opencv
    cv::Mat result_opencv;
    //sam_image2opencv(image_sam, result_opencv);
    sam_image_color2opencv(image_sam, result_opencv);

    cv::imwrite("/Users/rtous/dev/back2black/borrar2.jpg", result_opencv); 
}
