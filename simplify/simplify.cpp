#include "simplify.h"
#include "common1.h" 

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
    /*cv::RNG rng(12345);//random number generator    
    cv::Scalar color_with_alpha = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256), 255);
        
    if (randomColor) {
        color_with_alpha = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256), 255);
    } else {
        color_with_alpha = cv::Scalar( R, G, B, 255);
    }*/

    cv::Scalar color_with_alpha = cv::Scalar( std::rand()%256, std::rand()%256, std::rand()%256, 255);
    if (!randomColor) {
        color_with_alpha = cv::Scalar( B, G, R, 255);
    }
    printf("color_with_alpha = %.2lf, %.2lf, %.2lf\n", color_with_alpha[0], color_with_alpha[1], color_with_alpha[2]);

    for(int j = 0; j< contours.size(); j++ ) {
        cv::fillPoly(output_image, cv::Mat(contours[j]), color_with_alpha);
    }
}

std::vector<std::vector<cv::Point>> simplifyColorSegment(cv::Mat &mask, cv::Mat &output_image, bool randomColor, int R, int G, int B) 
{
        //cv::RNG rng(12345);//random number generator
        //int t = (unsigned)time(NULL);
        //t=std::rand()%1000;
        //printf("t = %d\n", t);
        //cv::RNG rng(t);
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
        

        /*cv::Scalar color_with_alpha = cv::Scalar( std::rand()%256, std::rand()%256, std::rand()%256, 255);
        if (!randomColor) {
            color_with_alpha = cv::Scalar( R, G, B, 255);
        }
        printf("Random color = %.2lf, %.2lf, %.2lf\n", color_with_alpha[0], color_with_alpha[1], color_with_alpha[2]);
        */

        //for eachcontour
        for( int j = 0; j< contours.size(); j++ )
        {
            int contour_area = cv::contourArea(contours[j]);
            printf("contour_area = %d\n", contour_area);
            if (contour_area > 200) {
                //DEBUG
                //cv::Mat drawing = cv::Mat::zeros( mask.size(), CV_8UC3 );
                //cv::drawContours( drawing, contours, (int)j, cv::Scalar(0, 255, 0), 2, cv::LINE_8, hierarchy, 0 );
                //cv::imwrite("output/example1/mask_contour"+std::to_string(j)+"_"+std::to_string(j)+".jpg", drawing);
                //

                /**** DILATE ****/

                //cv::dilate(img_bw, img_final, Mat(), Point(-1, -1), 2, 1, 1);

                /**** SIMPLIFY ***/

                std::vector<cv::Point> contoursOUT;
                cv::approxPolyDP( cv::Mat(contours[j]), contoursOUT, 4, false );
                //contours[j] = contoursOUT;
                simplifiedContours.push_back(contoursOUT);

                //DEBUG
                //cv::Mat drawing = cv::Mat::zeros( mask.size(), CV_8UC3 );
                //cv::drawContours(drawing, contours, (int)j, color, 2, cv::LINE_8, hierarchy, 0 );
                //cv::imwrite(output_path+"/mask_contour_simplified"+std::to_string(i)+"_"+std::to_string(j)+".jpg", drawing);

                /**** FILL ***/

                //DEBUG
                //cv::fillPoly(drawing, cv::Mat(contoursOUT), cv::Scalar(0, 255, 0));
                //cv::imwrite("output/example1/mask_contour_filled"+std::to_string(j)+"_"+std::to_string(j)+".jpg", drawing);
                //
            }
        }
        //We do this appart because the Editor saves just the contours and fills them to export the video
        
        fillContoursWithColorAndAlpha(simplifiedContours, output_image, randomColor, R, G, B);
        //cv::imshow("image", output_image);


        //DEBUG
        //cv::imwrite("output/example1/contour.png", output_image);
        //exit(0);
        //

        //cv::imwrite("output/example1/before_rimlight.png", output_image);
        //addRimLight(output_image, output_image, 5);

        return simplifiedContours;
        //cv::imwrite(output_path+"/output.png", output);
}

//img with color masks and transparent background
//WARNING: Currently the editor does not call this, it calls directly simplifyColorSegment
//         for each mask
void simplify(cv::Mat &input_image, cv::Mat &output_image) 
{
    printf("simplify...\n");
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
    cv::imwrite("output/example1/before_rimlight.png", output_image);
    //cv::Mat rimLightImage = cv::Mat::zeros(output_image.rows, output_image.cols, output_image.type());
    addRimLight(output_image, output_image, 5);
    //cv::imwrite("output/example1/with_rimlight.png", rimLightImage);



}


/*
def pixelate(input, w, h): # w,h  Desired "pixelated" size
    height, width = input.shape[:2]
    # Resize input to "pixelated" size
    temp = cv2.resize(input, (w, h),  interpolation=cv2.INTER_NEAREST)#cv2.INTER_LINEAR antialiasing
    # Initialize output image
    output = cv2.resize(temp, (width, height), interpolation=cv2.INTER_NEAREST)
    return output
*/

/*def change_brightness(img, value=100):
    _, _, _, a_channel = cv2.split(img)
    #img_BGRA = cv2.merge((b_channel, g_channel, r_channel, alpha_channel))
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    h, s, v = cv2.split(hsv)
    v = cv2.add(v,value)
    v[v > 255] = 255
    v[v < 0] = 0
    final_hsv = cv2.merge((h, s, v))
    img = cv2.cvtColor(final_hsv, cv2.COLOR_HSV2BGR)
    img = np.dstack((img, a_channel))
    return img

def addShadow(imcolor, shadowSize=10):    
    imcolor_result_shadow = imcolor.copy()
    height, width = imcolor_result_shadow.shape[:2]
    offsetx = shadowSize
    offsety = 0
    M = np.float32([[1, 0, offsetx], [0, 1, offsety]])
    dst_mat = np.zeros((height, width, 4), np.uint8)
    size = (width, height)
    imcolor_result_shadow = cv2.warpAffine(imcolor_result_shadow, M, size, dst_mat)
    imcolor_result_shadow = change_brightness(imcolor_result_shadow)
    result = overlay(bottomImage=imcolor_result_shadow, topImage=imcolor)
    return result
*/

/*
void overlay(cv::Mat &resultImage, cv::Mat &bottomImage, cv::Mat &topImage) { 
    //topImage = original bottomImage = bright displaced to the right
    
    //Convert the topImage into a mask
    cv::Mat mask;
    threshold(topImage, mask, 0, 255, cv::THRESH_BINARY);
    
    //Invert the mask
    cv::bitwise_not(mask, mask); 

    //Use the mask to cut the intersection from the bottomImage
    //erase the top image pixels  from the bottom image
    cv::Mat bottomImageMinusTopImage = cv::Mat::zeros(bottomImage.rows, bottomImage.cols, bottomImage.type());
    bottomImage.copyTo(bottomImageMinusTopImage, mask);

    //fill the missing pixels in the bottom image with the ones in the top image 
    resultImage = bottomImageMinusTopImage + topImage;
} */ 

void change_brightness(cv::Mat &input_image, cv::Mat &output_image) { 
    printf("change_brightness\n");
    //input_image.convertTo(output_image, -1, 1, 30);
    /*
    _, _, _, a_channel = cv2.split(img)
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    h, s, v = cv2.split(hsv)
    v = cv2.add(v,value)
    v[v > 255] = 255
    v[v < 0] = 0
    final_hsv = cv2.merge((h, s, v))
    img = cv2.cvtColor(final_hsv, cv2.COLOR_HSV2BGR)
    img = np.dstack((img, a_channel))
    */
    
    
    //cv::Mat bgra[4];   //output channels
    //cv::split(input_image, bgra);//split channels  
    //std::vector<cv::Mat> channels;
    //bgra[0] = bgra[0] + cv::Scalar(100);
    for( int y = 0; y < input_image.rows; y++ ) {
        for( int x = 0; x < input_image.cols; x++ ) {
            cv::Vec4b bgra = input_image.at<cv::Vec4b>(y,x);
            if (bgra[3] == 255) {
                bgra[0] = std::min(bgra[0]+50, 255); 
                bgra[1] = std::min(bgra[1]+50, 255); 
                bgra[2] = std::min(bgra[2]+50, 255);
            }
            output_image.at<cv::Vec4b>(y,x) = bgra;
        }
    }
    /*channels.push_back(bgra[0]);
    channels.push_back(bgra[1]);
    channels.push_back(bgra[2]);
    channels.push_back(bgra[3]);
    cv::merge(channels, input_image);// merge channels*/
    

    //output_image = input_image + cv::Scalar(100, 0, 0, 0);
    //output_image = input_image + cv::Scalar(-50, -50, -50);
}  

void debugInspectImage(cv::Mat &input_image) {  
    //https://codeyarns.com/tech/2015-08-27-depth-and-type-of-matrix-in-opencv.html#gsc.tab=0  
    for(int i=0; i<input_image.rows; i++) {
        for(int j=0; j<input_image.cols; j++) {
            if (input_image.type() == CV_8UC1) {
                printf("%u\n", input_image.at<uchar>(i,j));
            } else if (input_image.type() == CV_8UC3) {
                cv::Vec3b bgrPixel = input_image.at<cv::Vec3b>(i,j);
                printf("%u %u %u\n", bgrPixel[0], bgrPixel[1], bgrPixel[2]);

            } else if (input_image.type() == CV_8UC4) {
                cv::Vec4b bgrPixel = input_image.at<cv::Vec4b>(i,j);
                printf("%u %u %u %u\n", bgrPixel[0], bgrPixel[1], bgrPixel[2], bgrPixel[3]);

            }
            /*uchar color;
            Vec3b bgrPixel
            Vec4b bgraPixel
            pixel = img_gray.at<uchar>(i,j);*/
            
            //}
        }
    } 
}

void addRimLight(cv::Mat &input_image, cv::Mat &output_image, float rim_light_size) {    
    //Apply an image translation (a type of affine transform)
    float offsetx = rim_light_size;
    float offsety = 0;
    float warp_values[] = { 1.0, 0.0, offsetx, 0.0, 1.0, offsety };
    cv::Mat translation_matrix = cv::Mat(2, 3, CV_32F, warp_values);
    cv::Mat output_displaced_image = cv::Mat::zeros(input_image.rows, input_image.cols, input_image.type());
    
    //displace the image rim_light_size pixels to the right
    cv::warpAffine(input_image, output_displaced_image, translation_matrix, output_image.size());
    
    //add brightness to the displaced image (this should go below)
    change_brightness(output_displaced_image, output_displaced_image);
    
    //debugInspectImage(input_image);

    //add the original image over the displaced bright image (so only a rimlight remain)
    overlay(output_image, output_displaced_image, input_image);

}

