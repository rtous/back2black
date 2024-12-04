#include "segmentor_sam1.h"
#include "stdio.h"
#include <opencv2/imgcodecs.hpp>

int main()
{
    Segmentor& s = get_sam1_segmentor();  
    s.test();

    std::string image_path = "/Users/rtous/dev/back2black/img.jpg";
    cv::Mat opencv_image = imread(image_path, cv::IMREAD_COLOR);
    s.preprocessImage(opencv_image);
}
