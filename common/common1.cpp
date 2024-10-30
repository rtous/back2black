/*
    //conversion utils
    sam_image2color
    sam_image2opencv
    sam_image2opencv_color
    opencv_image2sam_binarymask
    opencv_image2sam
    
    //load utils
    load_image_samformat_from_file
    load_frames_from_files
    load_and_precompute_image_from_file

    //mask utils
    propagate_masks 
        -> compute_mask (also computes mask center, assumes precomputed)
            -> get_best_sam_mask_at_point (assumes precomputed)
    get_best_opencv_mask_at_point (NOT USED?)

    //other utils
    compute_mask_center (ONLY FIRST CONTOUR)
*/

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
    //Assumes that the image has been previously precomputed
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
//CURRENLTY USED??
cv::Mat get_best_opencv_mask_at_point(int x, int y, sam_image_u8 img0, sam_state & state, int n_threads) {
	
    //compute mask at given point (pick best one)
    sam_image_u8 mask;
    get_best_sam_mask_at_point(x, y, img0, state, n_threads, mask);
    
	//convert mask to opencv format
    cv::Mat mask_opencv;
    sam_image2opencv(mask, mask_opencv);

    return mask_opencv;
}

/*void example_func(Mask &aMask) {
    aMask.maskId = 1000;

}*/

//TODO: currently only the first contour
//Called by compute_mask_and_textures
void compute_mask_center(Mask & aMask) {
    //cv::Mat mask_opencv;
    sam_image2opencv(aMask.samMask, aMask.opencv_mask);
    printf("\tcompute_mask_center...\n");
    std::vector<cv::Vec4i> hierarchy;
    printf("\t\tfindContours...\n");
    cv:findContours(aMask.opencv_mask, aMask.contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE );
    if (aMask.contours.size() == 0) 
        printf("WARNING: No countours found for the mask!\n");
    //TODO: Multiple contours
    printf("\t\tcontourArea...\n");
    aMask.mask_contour_size = cv::contourArea(aMask.contours[0]);
    printf("new_contour_area = %d \n", aMask.mask_contour_size);

    // compute the center of the contour https://pyimagesearch.com/2016/02/01/opencv-center-of-contour/
    cv::Moments M = cv::moments(aMask.contours[0]);
    cv::Point center(M.m10/M.m00, M.m01/M.m00);  
    aMask.mask_center_x = center.x;
    aMask.mask_center_y = center.y;
    //circle(aMask.mask, center, 5, cv::Scalar(128,0,0), -1);//DEBUG
} 

//called by propagate_masks (called by editor and cli_masks)
//but the editor also calls compute_mask_and_textures in sam_utils.cpp
void compute_mask(Mask & aMask, sam_image_u8 img0, sam_state & state, int n_threads) {

    //Compute the frame: Obtain the best mask at the point
    //cv::Mat output = get_best_opencv_mask_at_point(aMask.mask_computed_at_x, aMask.mask_computed_at_y, img0, state, n_threads);
    
    //compute mask at given point (pick best one)
    get_best_sam_mask_at_point(aMask.mask_computed_at_x, aMask.mask_computed_at_y, img0, state, n_threads, aMask.samMask);
    
    compute_mask_center(aMask);

    aMask.mask_computed = true;

    /*
    //convert mask to opencv format
    sam_image2opencv(aMask.samMask, aMask.opencv_mask);

    //aMask.opencv_mask = get_best_opencv_mask_at_point(aMask.mask_computed_at_x, aMask.mask_computed_at_y, img0, state, n_threads);
    aMask.mask_computed = true;

    cv::Point center2(aMask.mask_computed_at_x, aMask.mask_computed_at_y); 
    circle(aMask.opencv_mask, center2, 5, cv::Scalar(128,128,0), -1);//DEBUG

    //aMask.opencv_mask = output;//DOES NOT WORKK output is a local variable?
    //cv::imwrite("output/example2/masks/test.png", aMask.opencv_mask);

    //Obtain the first contour
    //std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv:findContours(aMask.opencv_mask, aMask.contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE );
    if (aMask.contours.size() == 0) 
        printf("WARNING: No countours found for the mask!\n");

    //TODO: Multiple contours

    aMask.mask_contour_size = cv::contourArea(aMask.contours[0]);
    printf("new_contour_area = %d \n", aMask.mask_contour_size);

    // compute the center of the contour https://pyimagesearch.com/2016/02/01/opencv-center-of-contour/
    cv::Moments M = cv::moments(aMask.contours[0]);
    cv::Point center(M.m10/M.m00, M.m01/M.m00);  
    aMask.mask_center_x = center.x;
    aMask.mask_center_y = center.y;
    printf("aMask.mask_center_x=%d\n", aMask.mask_center_x);

    circle(aMask.opencv_mask, center, 5, cv::Scalar(128,0,0), -1);//DEBUG
    */
} 

/*void example_func() {
    int a = 2;
}*/

//If instead of a video the input is a directory of images
//it loads the images into the frames
//the filepaths are kept to later match output names but the images are loaded here
//currently only used by cli_masks
int load_frames_from_files(std::string input_path, std::vector<Frame> & frames) {
    if (!cv::utils::fs::exists(input_path)) {
        printf("Input directory does not exist: %s", input_path.c_str());
    }
    std::vector<std::string> filepaths_in_directory;
    cv::glob(input_path, filepaths_in_directory);
    int f = 0;
    for (std::string filepath : filepaths_in_directory) {
        std::cout << filepath << std::endl;
        std::string filename = filepath.substr(filepath.find_last_of("/\\") + 1);
        std::string filename_noext = filename.substr(0, filename.find_last_of(".")); 
        std::string extension = filename.substr(filename.find_last_of(".")+1); 

        if (extension == "jpg" || extension == "png") {
            Frame aFrame;
            aFrame.order = f;
            aFrame.filePath = filepath;
            //load the frame
            if (!load_image_samformat_from_file(filepath, aFrame.img_sam_format)) {
                fprintf(stderr, "%s: failed to load image from '%s'\n", __func__, filepath.c_str());
                return false;
            }
            fprintf(stderr, "%s: loaded image '%s' (%d x %d)\n", __func__, filepath.c_str(), aFrame.img_sam_format.nx, aFrame.img_sam_format.ny);
            frames.push_back(aFrame);
            f++; 
        }
    }
}

/*
OLD VERSION USED BY CLI_MASKS, computed also the mask of the first frame
//We assume the first frame has already the masks and the user coordinates
//but not the computation
//frames are in files
int propagate_masks(std::vector<Frame> & frames, sam_state & state, int n_threads) 
{
    int numFrames = frames.size();
    int f = 0;
    //iterate through all frames
    for (Frame & aFrame : frames) {
        printf("PROCESSING FRAME %d \n", f);
        
        //Pre-compute the frame 
        if (!sam_compute_embd_img(aFrame.img_sam_format, n_threads, state)) {
            fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
            return false;
        }
        printf("t_compute_img_ms = %d ms\n", state.t_compute_img_ms);
    

        //iterate through all the masks of the frame
        for (Mask & aMask : aFrame.masks) {
            printf("\tPROCESSING MASK %d \n", aMask.maskId);
            compute_mask(aMask, aFrame.img_sam_format, state, n_threads);
            
            //if there are previous masks can check if the mask makes sense:
            //bool isMaskConsistent = true; //th first mask is consistent
            //if (f > 0)
            //    isMaskConsistent =  mask contour similar to previous mask contour

            
            //add the mask to the next frame with the next coordinates
            printf("\tf=%d numFrames=%d.\n", f, numFrames);
            if (f < numFrames-1) {
                printf("\tADDING MASK %d TO FRAME %f.\n", aMask.maskId, f);
                Mask newMask;
                newMask.maskId = aMask.maskId;
                newMask.mask_computed_at_x = aMask.mask_center_x;
                newMask.mask_computed_at_y = aMask.mask_center_y; 
                frames[f+1].masks.push_back(newMask);
            }
                  //if isMaskConsistent new coordinates are the mask coordinates
                  //otherwise use given coordinates
            //    frames[f+1].addMask(new coordinates); 
            printf("\tMASKS DONE.\n");
        }
        printf("FRAME DONE.\n");
        f++;
    }
}
*/

//We assume the first frame has already the masks and the user coordinates
//and also the computation 
//frames are in memory
//ONGOING
//int propagate_masks(std::vector<Frame> & frames, sam_state & state, int n_threads) 
int propagate_masks(std::vector<Frame> & frames, sam_state & state, int n_threads, int from_frame, int till_frame) 
{
    printf("propagating from %d (reference) to %d\n", from_frame, till_frame);
    //int MAX = 5;
    int numFrames = frames.size();
    int f = 0;
    //iterate through all frames 
    //assumes that the first frame has been already computed
    for (Frame & aFrame : frames) {
        if (f > till_frame)
            break;
        printf("PROCESSING FRAME %d \n", f);
        //if not the first frame precompute the image
        if (f>from_frame) {
            printf("\t%d>from_frame so precomputing. \n", f);
            if (!sam_compute_embd_img(aFrame.img_sam_format, n_threads, state)) {
                fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
                return 1;
            }
        } 
        //iterate through all the masks of the frame
        if (f>=from_frame) {
            printf("\t%d>=from_frame so analyzing masks. \n", f);
            for (Mask & aMask : aFrame.masks) {
                printf("\tPROCESSING MASK %d \n", aMask.maskId);
                printf("\taMask.mask_computed_at_x %d \n", aMask.mask_computed_at_x);
                printf("\taMask.mask_computed_at_y %d \n", aMask.mask_computed_at_y);
                //if not the first frame compute the mask
                if (f>from_frame) { 
                    printf("\t%d>from_frame so computing mask. \n", f);
                    compute_mask(aMask, aFrame.img_sam_format, state, n_threads);
                }
                //compute mask center
                //compute_mask_center(aMask, aFrame.img_sam_format, state, n_threads);
                
                //add the mask to the next frame with the next coordinates
                printf("\tf=%d numFrames=%d.\n", f, numFrames);
                if (f < till_frame && f < numFrames-1) {
                    Mask newMask;
                    newMask.maskId = aMask.maskId;
                    newMask.color[0] = aMask.color[0];
                    newMask.color[1] = aMask.color[1];
                    newMask.color[2] = aMask.color[2];
                    newMask.mask_computed_at_x = aMask.mask_center_x;
                    newMask.mask_computed_at_y = aMask.mask_center_y; 
                    printf("\tADDING MASK %d TO FRAME %d WITH x=%d, y=%d\n", aMask.maskId, f, newMask.mask_computed_at_x, newMask.mask_computed_at_y);
                    frames[f+1].masks.push_back(newMask);
                }
                printf("\tMASKS DONE.\n");
            }
        }
        printf("FRAME DONE.\n");
        f++;
    }
    //precompute the original frame to keep things as they were
    /*if (!sam_compute_embd_img(frames[0].img_sam_format, n_threads, state)) {
        fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
        return 1;
    }*/
	return 0;
}
