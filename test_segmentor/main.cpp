#include "segmentor_sam1.h"
#include "segmentor_sam2.h"
#include "stdio.h"
#include <opencv2/imgcodecs.hpp>
#include "common1.h"

int main()
{
    ///////////////////////
    // test Segmentor SAM1
    ///////////////////////
    Segmentor& s = get_sam1_segmentor();  
    s.test();

    std::string image_path = "/Users/rtous/dev/back2black/img4.png";
    cv::Mat image_opencv = imread(image_path, cv::IMREAD_COLOR);

    s.preprocessImage(image_opencv);
    
    cv::Mat mask_opencv = s.get_best_mask_at_point(600, 450, image_opencv);
    cv::imwrite("/Users/rtous/dev/back2black/borrar_sam1.png", mask_opencv);   
    s.close();
    

    ///////////////////////
    // test Segmentor SAM2
    ///////////////////////
    Segmentor& s2 = get_sam2_segmentor();  
    s2.test();

    std::string image_path2 = "/Users/rtous/dev/back2black/img4.png";
    cv::Mat image_opencv2 = imread(image_path2, cv::IMREAD_COLOR);

    s2.preprocessImage(image_opencv2);
    
    cv::Mat mask_opencv2 = s2.get_best_mask_at_point(600, 450, image_opencv2);
    cv::imwrite("/Users/rtous/dev/back2black/borrar_sam2.png", mask_opencv2);   
    s2.close();
}
