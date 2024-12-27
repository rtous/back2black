#include "simplify.h"

/**
 * Splits a mask in N masks, one for each unique color
 *
 * This sum is the arithmetic sum, not some other kind of sum that only
 * mathematicians have heard of.
 * @param img_gray one channel image with the mask
 * @param unique_colors will store the result, a map of tuples <color_id, mask matrix>
 * @param pixels_per_colour stores the resulting size of each mask in pixels (no used?)
 * @return the number of unique colors
 */
int unique_colors(cv::Mat img_gray, std::map<int,cv::Mat> & unique_colors, std::map<int,int> & pixels_per_colour) 
{
    uchar color;
    int total_colors = 0;
    for(int i=0; i<img_gray.rows; i++) {
        for(int j=0; j<img_gray.cols; j++) {
            color = img_gray.at<uchar>(i,j);
            if (color != 0) {
                if (unique_colors.find(color) == unique_colors.end()) {
                    //New color detected
                    cv::Mat mask = cv::Mat::zeros(img_gray.size(), img_gray.type());
                    mask.at<uchar>(i,j) = 255;
                    unique_colors.insert(std::pair<int, cv::Mat>(color, mask));
                    pixels_per_colour.insert(std::pair<int, int>(color, 0));
                    total_colors++;
                } else {
                    cv::Mat mask = unique_colors.at(color);
                    mask.at<uchar>(i,j) = 255;
                    pixels_per_colour[color]++;
                }
            }
        }
    }
    return total_colors;
}

void fillContoursWithColorAndAlpha(std::vector<std::vector<cv::Point>> contours, cv::Mat &output_image, bool randomColor, int R, int G, int B)
{
    cv::RNG rng(12345);//random number generator    
    cv::Scalar color_with_alpha = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256), 255);
        
    if (randomColor) {
        color_with_alpha = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256), 255);
    } else {
        color_with_alpha = cv::Scalar( R, G, B, 255);
    }

    for(int j = 0; j< contours.size(); j++ ) {
        cv::fillPoly(output_image, cv::Mat(contours[j]), color_with_alpha);
    }
}

std::vector<std::vector<cv::Point>> simplifyColorSegment(cv::Mat &mask, cv::Mat &output_image, bool randomColor, int R, int G, int B) 
{
        cv::RNG rng(12345);//random number generator
        //cv::imwrite(output_path+"/mask"+std::to_string(i)+".jpg", mask);

        /**** DILATE ****/
        cv::dilate(mask, mask, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4)), cv::Point(-1, -1), 1, 1, 1);

        /******** FIND CONTOURS *********/
        std::vector<std::vector<cv::Point>> contours;
        std::vector<std::vector<cv::Point>> simplifiedContours;
        std::vector<cv::Vec4i> hierarchy;
        cv:findContours(mask, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE );

        //TODO: generate a random color for all contours of a color
        //cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        

        cv::Scalar color_with_alpha = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256), 255);
        
        if (randomColor) {
            color_with_alpha = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256), 255);
        } else {
            color_with_alpha = cv::Scalar( R, G, B, 255);
        }

        //for eachcontour
        for( int j = 0; j< contours.size(); j++ )
        {
            int contour_area = cv::contourArea(contours[j]);
            printf("contour_area = %d\n", contour_area);
            if (contour_area > 200) {
                //cv::Mat drawing = cv::Mat::zeros( mask.size(), CV_8UC3 );
                //cv::drawContours( drawing, contours, (int)j, color, 2, cv::LINE_8, hierarchy, 0 );
                //cv::imwrite(output_path+"/mask_contour"+std::to_string(i)+"_"+std::to_string(j)+".jpg", drawing);


                /**** DILATE ****/

                //cv::dilate(img_bw, img_final, Mat(), Point(-1, -1), 2, 1, 1);

                /**** SIMPLIFY ***/

                std::vector<cv::Point> contoursOUT;
                cv::approxPolyDP( cv::Mat(contours[j]), contoursOUT, 4, false );
                //contours[j] = contoursOUT;
                simplifiedContours.push_back(contoursOUT);

                //drawing = cv::Mat::zeros( mask.size(), CV_8UC3 );
                //cv::drawContours(drawing, contours, (int)j, color, 2, cv::LINE_8, hierarchy, 0 );
                //cv::imwrite(output_path+"/mask_contour_simplified"+std::to_string(i)+"_"+std::to_string(j)+".jpg", drawing);

                /**** FILL ***/

                //cv2.fillPoly(imcolor, pts =[contour], color=display_color)
                //cv::fillPoly( output_image, cv::Mat(contours[j]), color_with_alpha);
                //cv::imwrite("mask_contour_filled"+std::to_string(i)+"_"+std::to_string(j)+".jpg", drawing);

            }
        }
        //We do this appart because the Editor saves just the contours and fills them to export the video
        fillContoursWithColorAndAlpha(simplifiedContours, output_image, randomColor, R, G, B);
        return simplifiedContours;
        //cv::imwrite(output_path+"/output.png", output);
}

//img with color masks and transparent background
void simplify(cv::Mat &input_image, cv::Mat &output_image) 
{
    /******* TO GRAYSCALE *********/
    cv::Mat img_gray;
    cv::cvtColor(input_image, img_gray, cv::COLOR_BGR2GRAY);

    /******** COLOR SEGMENTS *********/
    //int nc = unique_colors(img);
    std::map<int,cv::Mat> unique_colors_map;//color ids and masks
    std::map<int,int> pixels_per_colour;
    int nc = unique_colors(img_gray, unique_colors_map, pixels_per_colour);
    //nc = number of total colors
    //unique_colors_map contains tuples <color_id, mask matrix>
    printf("Total colors = %d.\n", nc);

    /******* DILATE MASKS *****/
    //dilate_masks(unique_colors_map);

    /******** CONTOURS *********/
    int i = 0;
    //for each color segment
    cv::RNG rng(12345);//random number generator
    for (std::map<int,cv::Mat>::iterator it=unique_colors_map.begin(); it!=unique_colors_map.end(); ++it) {
        int segment_color = it->first;
        printf("Writing mask for color %d\n", segment_color);
        if (pixels_per_colour[it->first] > 500) {


            cv::Mat mask = it->second;

            simplifyColorSegment(mask, output_image, true, -1, -1, -1);

            /*
            //cv::imwrite(output_path+"/mask"+std::to_string(i)+".jpg", mask);

            ///// DILATE /////
            cv::dilate(mask, mask, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4)), cv::Point(-1, -1), 1, 1, 1);
    
            ///// FIND CONTOURS /////
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
                    //cv::imwrite(output_path+"/mask_contour"+std::to_string(i)+"_"+std::to_string(j)+".jpg", drawing);


                    ///// DILATE /////

                    //cv::dilate(img_bw, img_final, Mat(), Point(-1, -1), 2, 1, 1);

                    ///// SIMPLIFY /////

                    std::vector<cv::Point> contoursOUT;
                    cv::approxPolyDP( cv::Mat(contours[j]), contoursOUT, 4, false );
                    contours[j] = contoursOUT;

                    drawing = cv::Mat::zeros( mask.size(), CV_8UC3 );
                    cv::drawContours(drawing, contours, (int)j, color, 2, cv::LINE_8, hierarchy, 0 );
                    //cv::imwrite(output_path+"/mask_contour_simplified"+std::to_string(i)+"_"+std::to_string(j)+".jpg", drawing);

                    ///// FILL /////

                    //cv2.fillPoly(imcolor, pts =[contour], color=display_color)
                    cv::fillPoly( output_image, cv::Mat(contours[j]), color_with_alpha);
                    //cv::imwrite("mask_contour_filled"+std::to_string(i)+"_"+std::to_string(j)+".jpg", drawing);

                }
            }
            */
            //cv::imwrite(output_path+"/output.png", output);
            
            i++;
        }
    }
}
