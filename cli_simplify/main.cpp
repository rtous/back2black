#include <stdio.h>
#include <opencv2/opencv.hpp> 
#include <opencv2/core/utils/filesystem.hpp>

#include "simplify.h"


// Main code
int main(int argc, char ** argv) 
{
    printf("CLI SIMPLIFY tool v 0.1\n");

    std::string input_path = "data/example1/masks";
    std::string output_path = "output/example1";

    if (!cv::utils::fs::exists(output_path)) {
        printf("Output directory does not exist, creating: %s", output_path.c_str());
        cv::utils::fs::createDirectories(output_path);
    }

    /****** LOAD IMAGE **********/
    //im = cv2.imread(os.path.join(inputpath, filename))
    //cv::Mat img = cv::imread("img.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img = cv::imread(input_path+"/001.png");
    //cv::Mat output = cv::Mat::zeros( img.size(), CV_8UC3 );
    cv::Mat output = cv::Mat::zeros( img.size(), CV_8UC4 ); //includes alpha channel
    
    
    if (img.empty()) { 
        printf("Image not found.");
        return -1; 
    } else {
        printf("Image successfully read.\n");
    }

    simplify(img, output);

    /*
    ///////// TO GRAYSCALE /////////
    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

    ///////// COLOR SEGMENTS /////////
    //int nc = unique_colors(img);
    std::map<int,cv::Mat> unique_colors_map;//color ids and masks
    std::map<int,int> pixels_per_colour;
    int nc = unique_colors(img_gray, unique_colors_map, pixels_per_colour);
    //nc = number of total colors
    //unique_colors_map contains tuples <color_id, mask matrix>
    printf("Total colors = %d.\n", nc);

    ///////// DILATE MASKS /////////
    //dilate_masks(unique_colors_map);


    ///////// CONTOURS /////////
    int i = 0;
    //for each color segment
    cv::RNG rng(12345);//random number generator
    for (std::map<int,cv::Mat>::iterator it=unique_colors_map.begin(); it!=unique_colors_map.end(); ++it) {
        int segment_color = it->first;
        printf("Writing mask for color %d\n", segment_color);
        if (pixels_per_colour[it->first] > 500) {
            cv::Mat mask = it->second;
            cv::imwrite(output_path+"/mask"+std::to_string(i)+".jpg", mask);

            ///////// DILATE /////////
            cv::dilate(mask, mask, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4)), cv::Point(-1, -1), 1, 1, 1);
    
            ///////// FIND CONTOURS /////////
            std::vector<std::vector<cv::Point> > contours;
            std::vector<cv::Vec4i> hierarchy;
            cv:findContours(mask, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE );

            //TODO: generate a random color for all contours of a color
            cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
            cv::Scalar color_with_alpha = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256), 255);
            
            //for eachcontour
            for( int j = 0; j< contours.size(); j++ )
            {
                if (cv::contourArea(contours[j]) > 200) {
                    cv::Mat drawing = cv::Mat::zeros( mask.size(), CV_8UC3 );
                    cv::drawContours( drawing, contours, (int)j, color, 2, cv::LINE_8, hierarchy, 0 );
                    cv::imwrite(output_path+"/mask_contour"+std::to_string(i)+"_"+std::to_string(j)+".jpg", drawing);


                    ///////// DILATE /////////

                    //cv::dilate(img_bw, img_final, Mat(), Point(-1, -1), 2, 1, 1);

                    ///////// SIMPLIFY /////////

                    std::vector<cv::Point> contoursOUT;
                    cv::approxPolyDP( cv::Mat(contours[j]), contoursOUT, 4, false );
                    contours[j] = contoursOUT;

                    drawing = cv::Mat::zeros( mask.size(), CV_8UC3 );
                    cv::drawContours(drawing, contours, (int)j, color, 2, cv::LINE_8, hierarchy, 0 );
                    cv::imwrite(output_path+"/mask_contour_simplified"+std::to_string(i)+"_"+std::to_string(j)+".jpg", drawing);

                    ///////// FILL /////////

                    //cv2.fillPoly(imcolor, pts =[contour], color=display_color)
                    cv::fillPoly( output, cv::Mat(contours[j]), color_with_alpha);
                    //cv::imwrite("mask_contour_filled"+std::to_string(i)+"_"+std::to_string(j)+".jpg", drawing);

                }
            }
            
            cv::imwrite(output_path+"/output.png", output);

            i++;
        }
    }*/
    
    return 0;
}
