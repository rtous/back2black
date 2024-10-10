#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include "common1.h"
#include "filesystem_header.h"
#include <opencv2/opencv.hpp> 
#include <opencv2/core/utils/filesystem.hpp>
#include "sam.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

sam_image_u8 sam_image2color(sam_image_u8 & sam_image) {
    //Replicates the intensity (pressumably 0 or 1) to all channels, including alpha
    //Used before calling createGLTexture 
    sam_image_u8 mask_rgb = { sam_image.nx, sam_image.ny, };
    mask_rgb.data.resize(4*sam_image.nx*sam_image.ny);
    for (int i = 0; i < sam_image.nx*sam_image.ny; ++i) {
        mask_rgb.data[4*i+0] = sam_image.data[i];
        mask_rgb.data[4*i+1] = sam_image.data[i];
        mask_rgb.data[4*i+2] = sam_image.data[i];
        mask_rgb.data[4*i+3] = sam_image.data[i];
        /*if (sam_image.data[i] == 0)
            mask_rgb.data[4*i+3] = sam_image.data[i];
        else
            mask_rgb.data[4*i+3] = 255;
        */
    }
    return mask_rgb;
}

void sam_image2opencv(sam_image_u8 & sam_image, cv::Mat & opencv_image) {
    //WARNING: Only works for grayscale image (look at sam_image2opencv_color otherwise)
    //cv::Mat rows x columns 
    //sam_image_u8 guarda com si recorre tot per fileres
    opencv_image = cv::Mat::zeros(sam_image.ny, sam_image.nx, CV_8UC1 );
    for (int i=0; i < opencv_image.rows; ++i){
        for (int j=0; j < opencv_image.cols; ++j){
            opencv_image.at<uchar>(i, j) = sam_image.data[i*sam_image.nx+j];
        }
    }
}

void sam_image2opencv_color(sam_image_u8 & sam_image, cv::Mat & opencv_image, int R, int G, int B) {
    //Input sam mask (grayscale)
    //Output OpenCV color 4 channels with the indicated color (when mask>0)
    
    //cv::Mat rows x columns 
    //sam_image_u8 guarda com si recorre tot per fileres
    opencv_image = cv::Mat::zeros(sam_image.ny, sam_image.nx, CV_8UC4 );
    
    //For CV_8UC1: uchar pixelGrayValue = image.at<uchar>(r,c).
    //For CV_8UC3: cv::Vec3b pixelColor = image.at<cv::Vec3b>(r,c). The cv::Vec3b object represents a triplet of uchar values (integers between 0 and 255).

    for (int i=0; i < opencv_image.rows; ++i){
        for (int j=0; j < opencv_image.cols; ++j){
            uchar pixelGrayValue = sam_image.data[i*sam_image.nx+j];
            cv::Vec4b pixelColor; //typedef Vec<uchar, 3> Vec3b;
            if (pixelGrayValue>0) {
                pixelColor[0] = B;
                pixelColor[1] = G;
                pixelColor[2] = R;
            } else {
                pixelColor[0] = pixelGrayValue;
                pixelColor[1] = pixelGrayValue;
                pixelColor[2] = pixelGrayValue;
            }
            pixelColor[3] = 255; //alpha = opacity
            
            //opencv_image.at<uchar>(i, j) = sam_image.data[i*sam_image.nx+j];
            opencv_image.at<cv::Vec4b>(i, j) = pixelColor;
        }
    }
}

/*
void sam_image2opencv_color(sam_image_u8 & sam_image, cv::Mat & opencv_image) {
    //TODO: Only works for grayscale image
    //cv::Mat rows x columns 
    //sam_image_u8 guarda com si recorre tot per fileres
    //cv::Mat mask_opencv = cv::Mat::zeros(sam_image.ny, sam_image.nx, CV_8UC3);
    //opencv_image = cv::Mat::zeros(sam_image.ny, sam_image.nx, CV_8UC3 );
    opencv_image = cv::Mat::zeros(sam_image.ny, sam_image.nx, CV_8UC4);

    for (int i=0; i < opencv_image.rows; ++i){
        for (int j=0; j < opencv_image.cols; ++j){
            opencv_image.at<uchar>(i, j) = sam_image.data[i*sam_image.nx+j];
        }
    }
}*/

//Given a grayscale or color OpenCV image, it translates it into a binary mask in sam format
//Intensities become 0 or 255
//Used in compute_masks in sam_utils.cpp 
void opencv_image2sam_binarymask(sam_image_u8 & sam_image, cv::Mat & opencv_image) {
    
    //Convert default OpenCV BGR to GRAYSCALE
    cv::cvtColor(opencv_image, opencv_image, cv::COLOR_BGR2GRAY);

    //SAM x=width, y=height
    sam_image.nx = opencv_image.cols;
    sam_image.ny = opencv_image.rows;

    //TODO IMPROVEMENT: Sequential access to opencv_image.data 

    sam_image.data.clear();
    for (int i=0; i < opencv_image.rows; ++i){
        for (int j=0; j < opencv_image.cols; ++j){  
            //cv::Vec3b RGB = opencv_image.at<cv::Vec3b>(i, j);
            uchar intensity = opencv_image.at<uchar>(i, j);
            if (intensity > 0)
                intensity = 255;
            sam_image.data.push_back(intensity);
        }
    }
}

//Given a color OpenCV image, it translates it into an image in sam format
//Not used!?!?! not sure if works properly
void opencv_image2sam(sam_image_u8 & sam_image, cv::Mat & opencv_image) {
    //Convert default OpenCV BGR to RGB
    cv::cvtColor(opencv_image, opencv_image, cv::COLOR_BGR2RGB);

    //SAM x=width, y=height
    sam_image.nx = opencv_image.cols;
    sam_image.ny = opencv_image.rows;

    //TODO IMPROVEMENT: Sequential access to opencv_image.data 

    sam_image.data.clear();
    for (int i=0; i < opencv_image.rows; ++i){
        for (int j=0; j < opencv_image.cols; ++j){  
            cv::Vec3b RGB = opencv_image.at<cv::Vec3b>(i, j);
            //uchar intensity = opencv_image.at<uchar>(i, j);
            sam_image.data.push_back(RGB[0]);
            sam_image.data.push_back(RGB[1]);
            sam_image.data.push_back(RGB[2]);
        }
    }
}


bool load_image_samformat_from_file(const std::string & fname, sam_image_u8 & img) {
    int nx, ny, nc;
    auto data = stbi_load(fname.c_str(), &nx, &ny, &nc, 3);
    if (!data) {
        fprintf(stderr, "%s: failed to load '%s'\n", __func__, fname.c_str());
        return false;
    }
    if (nc != 3) {
        fprintf(stderr, "%s: '%s' has %d channels (expected 3)\n", __func__, fname.c_str(), nc);
        return false;
    }

    img.nx = nx;
    img.ny = ny;
    img.data.resize(nx * ny * 3);
    memcpy(img.data.data(), data, nx * ny * 3);

    stbi_image_free(data);

    return true;
}

/*
    load and precompute the image
*/
bool load_and_precompute_image_from_file(std::string path, sam_image_u8 & img0, sam_state & state, int n_threads) {
    //Load the image
    if (!load_image_samformat_from_file(path, img0)) {
        fprintf(stderr, "%s: failed to load image from '%s'\n", __func__, path.c_str());
        return false;
    }
    fprintf(stderr, "%s: loaded image '%s' (%d x %d)\n", __func__, path.c_str(), img0.nx, img0.ny);

    //Pre-compute the frame 
    if (!sam_compute_embd_img(img0, n_threads, state)) {
        fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
        return false;
    }
    printf("t_compute_img_ms = %d ms\n", state.t_compute_img_ms);
    return true;
}

/*
    given an image (in sam format) get the best mask (in sam format at a given point 
    return false if no mask has been found
*/
bool get_best_sam_mask_at_point(int x, int y, sam_image_u8 img0, sam_state & state, int n_threads, sam_image_u8 & mask) {
    //Call sam to compute the mask at the point
    std::vector<sam_image_u8> masks;
    sam_point pt {(float)x, (float)y};
    masks = sam_compute_masks(img0, n_threads, pt, state);
    printf("[INFO] found %d masks\n", masks.size());
    if (masks.size()>0) {
        mask = masks[0]; //the returned masks are ordered by iou and stability_score
        return true;
    } else {
        return false;
    }
}

/*
	given an image (in sam format) get the best mask (in opencv format at a given point 
*/
cv::Mat get_best_opencv_mask_at_point(int x, int y, sam_image_u8 img0, sam_state & state, int n_threads) {
	
    //compute mask at given point (pick best one)
    sam_image_u8 mask;
    get_best_sam_mask_at_point(x, y, img0, state, n_threads, mask);
    
	//convert mask to opencv format
    cv::Mat mask_opencv;
    sam_image2opencv(mask, mask_opencv);

    return mask_opencv;
}

/*void example_func(Object &anObject) {
    anObject.objectId = 1000;

}*/

//TODO
void compute_object_mask_center(Object & anObject, sam_image_u8 img0, sam_state & state, int n_threads) {
    cv::Mat mask_opencv;
    sam_image2opencv(anObject.samMask, mask_opencv);
    printf("\tcompute_object_mask_center...\n");
    std::vector<cv::Vec4i> hierarchy;
    printf("\t\tfindContours...\n");
    cv:findContours(mask_opencv, anObject.contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE );
    if (anObject.contours.size() == 0) 
        printf("WARNING: No countours found for the mask!\n");
    //TODO: Multiple contours
    printf("\t\tcontourArea...\n");
    anObject.mask_contour_size = cv::contourArea(anObject.contours[0]);
    printf("new_contour_area = %d \n", anObject.mask_contour_size);

    // compute the center of the contour https://pyimagesearch.com/2016/02/01/opencv-center-of-contour/
    cv::Moments M = cv::moments(anObject.contours[0]);
    cv::Point center(M.m10/M.m00, M.m01/M.m00);  
    anObject.mask_center_x = center.x;
    anObject.mask_center_y = center.y;
    //circle(anObject.mask, center, 5, cv::Scalar(128,0,0), -1);//DEBUG
} 

void compute_object(Object & anObject, sam_image_u8 img0, sam_state & state, int n_threads) {

    //Compute the frame: Obtain the best mask at the point
    //cv::Mat output = get_best_opencv_mask_at_point(anObject.mask_computed_at_x, anObject.mask_computed_at_y, img0, state, n_threads);
    anObject.mask = get_best_opencv_mask_at_point(anObject.mask_computed_at_x, anObject.mask_computed_at_y, img0, state, n_threads);
    
    cv::Point center2(anObject.mask_computed_at_x, anObject.mask_computed_at_y); 
    circle(anObject.mask, center2, 5, cv::Scalar(128,128,0), -1);//DEBUG

    //anObject.mask = output;//DOES NOT WORKK output is a local variable?
    //cv::imwrite("output/example2/masks/test.png", anObject.mask);

    //Obtain the first contour
    //std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv:findContours(anObject.mask, anObject.contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE );

    //TODO: Multiple contours

    anObject.mask_contour_size = cv::contourArea(anObject.contours[0]);
    printf("new_contour_area = %d \n", anObject.mask_contour_size);

    // compute the center of the contour https://pyimagesearch.com/2016/02/01/opencv-center-of-contour/
    cv::Moments M = cv::moments(anObject.contours[0]);
    cv::Point center(M.m10/M.m00, M.m01/M.m00);  
    anObject.mask_center_x = center.x;
    anObject.mask_center_y = center.y;
    circle(anObject.mask, center, 5, cv::Scalar(128,0,0), -1);//DEBUG
} 

/*void example_func() {
    int a = 2;
}*/
 
//We assume the first frame has already the objects and the user coordinates
//but not the computation
//frames are in files
int propagate_masks(std::vector<Frame> & frames, sam_state & state, int n_threads) 
{
    int numFrames = frames.size();
    int f = 0;
    //iterate through all frames
    for (Frame & aFrame : frames) {
        printf("PROCESSING FRAME %d \n", f);
        //load the frame
        sam_image_u8 img0;
        if (!load_and_precompute_image_from_file(aFrame.filePath, img0, state, n_threads)) {
            fprintf(stderr, "%s: failed load_and_precompute_image_from_file from '%s'\n", __func__, aFrame.filePath.c_str());
            return 1;
        }

        //iterate through all the objects of the frame
        for (Object & anObject : aFrame.objects) {
            printf("\tPROCESSING OBJECT %d \n", anObject.objectId);
            compute_object(anObject, img0, state, n_threads);
            
            //if there are previous objects can check if the mask makes sense:
            //bool isMaskConsistent = true; //th first mask is consistent
            //if (f > 0)
            //    isMaskConsistent =  mask contour similar to previous mask contour

            
            //add the object to the next frame with the next coordinates
            printf("\tf=%d numFrames=%d.\n", f, numFrames);
            if (f < numFrames-1) {
                printf("\tADDING OBJECT %d TO FRAME %f.\n", anObject.objectId, f);
                Object newObject;
                newObject.objectId = anObject.objectId;
                newObject.mask_computed_at_x = anObject.mask_center_x;
                newObject.mask_computed_at_y = anObject.mask_center_y; 
                frames[f+1].objects.push_back(newObject);
            }
                  //if isMaskConsistent new coordinates are the mask coordinates
                  //otherwise use given coordinates
            //    frames[f+1].addObject(new coordinates); 
            printf("\tOBJECTS DONE.\n");
        }
        printf("FRAME DONE.\n");
        f++;
    }
}

//We assume the first frame has already the objects and the user coordinates
//and also the computation 
//frames are in memory
//ONGOING
int propagate_masks2(std::vector<Frame> & frames, sam_state & state, int n_threads) 
{
    int numFrames = frames.size();
    int f = 0;
    //iterate through all frames
    for (Frame & aFrame : frames) {
        printf("PROCESSING FRAME %d \n", f);
        if (f>0) {//if not the first frame
            //precompute frame
            if (!sam_compute_embd_img(aFrame.img_sam_format, n_threads, state)) {
                fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
                //return 1;
            }
        }

        //iterate through all the objects of the frame
        for (Object & anObject : aFrame.objects) {
            printf("\tPROCESSING OBJECT %d \n", anObject.objectId);
            if (f>0) { //if not the first frame
                compute_object(anObject, aFrame.img_sam_format, state, n_threads);
            }
            compute_object_mask_center(anObject, aFrame.img_sam_format, state, n_threads);
            //add the object to the next frame with the next coordinates
            printf("\tf=%d numFrames=%d.\n", f, numFrames);
            if (f < numFrames-1) {
                Object newObject;
                newObject.objectId = anObject.objectId;
                newObject.mask_computed_at_x = anObject.mask_center_x;
                newObject.mask_computed_at_y = anObject.mask_center_y; 
                printf("\tADDING OBJECT %d TO FRAME %d WITH x=%d, y=%d\n", anObject.objectId, f, newObject.mask_computed_at_x, newObject.mask_computed_at_y);
                frames[f+1].objects.push_back(newObject);
            }
            printf("\tOBJECTS DONE.\n");
        }
        printf("FRAME DONE.\n");
        f++;
    }
}
