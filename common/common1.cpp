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
#include "face.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

sam_image_u8 sam_mask_to_sam_4channels(sam_image_u8 & sam_image, int alpha) {
    //Replicates the intensity (pressumably 0 or 1) to all channels, including alpha
    //Used before calling createGLTexture 
    sam_image_u8 mask_rgb = { sam_image.nx, sam_image.ny, };
    mask_rgb.data.resize(4*sam_image.nx*sam_image.ny);
    for (int i = 0; i < sam_image.nx*sam_image.ny; ++i) {
        mask_rgb.data[4*i+0] = sam_image.data[i];
        mask_rgb.data[4*i+1] = sam_image.data[i];
        mask_rgb.data[4*i+2] = sam_image.data[i];
        if (sam_image.data[i] == 0)
            mask_rgb.data[4*i+3] = 0;
        else
            mask_rgb.data[4*i+3] = alpha;
        /*if (sam_image.data[i] == 0)
            mask_rgb.data[4*i+3] = sam_image.data[i];
        else
            mask_rgb.data[4*i+3] = 255;
        */
    }
    return mask_rgb;
}

//sam image (grayscale) -> opencv (grayscale)
//returns a 1-channel image
void sam_image_grayscale2opencv(sam_image_u8 & sam_image, cv::Mat & opencv_image) {
    //WARNING: Only works for grayscale image (look at sam_image2opencv_color otherwise)
    //cv::Mat rows x columns 
    //sam_image_u8 guarda com si recorre tot per fileres
    opencv_image = cv::Mat::zeros(sam_image.ny, sam_image.nx, CV_8UC1 );
    for (int i=0; i < opencv_image.rows; ++i){
        for (int j=0; j < opencv_image.cols; ++j){
            opencv_image.at<uchar>(i, j) = sam_image.data[i*sam_image.nx+j];
            if (i==0 and j<10) printf("sam_image.data[i*sam_image.nx+j]=%d\n", sam_image.data[i*sam_image.nx+j]); //DEBUG
        }
    }
}

//sam image (RGB) -> opencv (BGR)
//NOTE: ignoring the fact that sam stores pixels as floats 
//USAGE: Only for debugging (by reversing opencv_image2sam)
//USAGE: Now used also in segmentor_sam2.cpp
void sam_image_color2opencv(sam_image_u8 & sam_image, cv::Mat & opencv_image) {
    //cv::Mat rows x columns 
    //sam_image_u8 guarda com si recorre tot per fileres
    opencv_image = cv::Mat::zeros(sam_image.ny, sam_image.nx, CV_8UC3);
    for (int i=0; i < opencv_image.rows; ++i){
        for (int j=0; j < opencv_image.cols; ++j){
            cv::Vec3b & color = opencv_image.at<cv::Vec3b>(i, j);
            color[0] = sam_image.data[3*(i*sam_image.nx+j)+2];
            color[1] = sam_image.data[3*(i*sam_image.nx+j)+1];
            color[2] = sam_image.data[3*(i*sam_image.nx+j)+0];
            //color[3] = 255;
        }
    }
}


//colorizes a mask
//WARNING: NOT USAGE DETECTED!!
void sam_image_grayscale2opencv_colorize(sam_image_u8 & sam_image, cv::Mat & opencv_image, int R, int G, int B) {
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
//WARNING: Binarizes values (0..255) into 0/255. Currently threshold=130
void opencv_image2sam_binarymask(sam_image_u8 & sam_image, cv::Mat & opencv_image) {
    cv::Mat opencv_image_gray;
    if (opencv_image.channels() > 1) {
        //Convert default OpenCV BGR to GRAYSCALE
        printf("WARNING: Convert default OpenCV BGR to GRAYSCALE in opencv_image2sam_binarymask\n");
        cv::cvtColor(opencv_image, opencv_image_gray, cv::COLOR_BGR2GRAY);
    } else {
        opencv_image_gray = opencv_image; // shallow copy ok
    }
    //SAM x=width, y=height
    sam_image.nx = opencv_image_gray.cols;
    sam_image.ny = opencv_image_gray.rows;

    //TODO IMPROVEMENT: Sequential access to opencv_image.data 

    sam_image.data.clear();
    for (int i=0; i < opencv_image_gray.rows; ++i){
        for (int j=0; j < opencv_image_gray.cols; ++j){  
            //cv::Vec3b RGB = opencv_image.at<cv::Vec3b>(i, j);
            uchar intensity = opencv_image_gray.at<uchar>(i, j);
            //if (intensity > 0)
            //    intensity = 255;
            if (i==0 and j<10) printf("intensity=%d\n", intensity); //DEBUG
            //binarize
            intensity = (intensity > 130 ? 255 : 0); //values between 0 and 255 -> 0 or 255
            //if (i==0 and j<10)
            //    printf("intensity=%d\n", intensity);
            sam_image.data.push_back(intensity);
        }
    }
}

//Given a color OpenCV image, it translates it into an image in sam format
//Not used!?!?! not sure if works properly
//TODO: It does not work I guess...
//Potentially used to convert the face -> sam -> GLTexture
//But trying directly face -> GLTexture
//WARNING: The 4 channels logic is not working. 
//         Not sure if it's related to the sam image of to 
//         createGLtexture
//         It's also duplicated in "opencv_image4channels_to_sam4channels"
//         But I'm using directly createGLTextureOpenCV (gui_utils.cpp) which works with 4 channels 
void opencv_image2sam(sam_image_u8 & sam_image, cv::Mat & opencv_image) {
    cv::Mat converted;
    if (opencv_image.channels() == 3) {
        //Convert default OpenCV BGR to RGB
        cv::cvtColor(opencv_image, converted, cv::COLOR_BGR2RGB);
    } else if (opencv_image.channels() == 4) {
        cv::cvtColor(opencv_image, converted, cv::COLOR_BGRA2RGBA);
    } else {
        printf("WARNING: only %d channels detected in opencv_image2sam.\n", opencv_image.channels());
    }

    //SAM x=width, y=height
    sam_image.nx = converted.cols;
    sam_image.ny = converted.rows;

    //TODO IMPROVEMENT: Sequential access to opencv_image.data 

    sam_image.data.clear();
    for (int i=0; i < converted.rows; ++i){
        for (int j=0; j < converted.cols; ++j){
            if (converted.channels() == 3) {
                cv::Vec3b RGB = converted.at<cv::Vec3b>(i, j);
                //uchar intensity = opencv_image.at<uchar>(i, j);
                sam_image.data.push_back(RGB[0]);
                sam_image.data.push_back(RGB[1]);
                sam_image.data.push_back(RGB[2]);
            } else if (converted.channels() == 4) {
                cv::Vec4b RGBA = converted.at<cv::Vec4b>(i, j);
                //uchar intensity = opencv_image.at<uchar>(i, j);
                sam_image.data.push_back(RGBA[0]);
                sam_image.data.push_back(RGBA[1]);
                sam_image.data.push_back(RGBA[2]);
                sam_image.data.push_back(RGBA[3]);
            } else {
                printf("WARNING: only %d channels detected in opencv_image2sam.\n", converted.channels());
            }
        }
    }
}


//Under development (probably not working yet)
//To be used in sam_utils.cpp/compute_mask_textures
void opencv_image4channels_to_sam4channels(sam_image_u8 & sam_image, cv::Mat & opencv_image) {
    //Convert default OpenCV BGR to RGB
    cv::cvtColor(opencv_image, opencv_image, cv::COLOR_BGRA2RGBA);

    //SAM x=width, y=height
    sam_image.nx = opencv_image.cols;
    sam_image.ny = opencv_image.rows;
    sam_image.data.resize(sam_image.nx * sam_image.ny * 4);

    //TODO IMPROVEMENT: Sequential access to opencv_image.data 

    sam_image.data.clear();
    for (int i=0; i < opencv_image.rows; ++i){
        for (int j=0; j < opencv_image.cols; ++j){  
            cv::Vec4b RGBA = opencv_image.at<cv::Vec4b>(i, j);
            //uchar intensity = opencv_image.at<uchar>(i, j);
            sam_image.data.push_back(RGBA[0]);
            sam_image.data.push_back(RGBA[1]);
            sam_image.data.push_back(RGBA[2]);
            sam_image.data.push_back(RGBA[3]); 
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
    bool found = get_best_sam_mask_at_point(x, y, img0, state, n_threads, mask);
    
	//convert mask to opencv format
    cv::Mat mask_opencv;
    sam_image_grayscale2opencv(mask, mask_opencv);

    return mask_opencv;
}

/*void example_func(Mask &aMask) {
    aMask.maskId = 1000;

}*/

//TODO: currently only the first contour
//Called by compute_mask_and_textures
void compute_mask_center(Mask & aMask) {
    //cv::Mat mask_opencv;
    sam_image_grayscale2opencv(aMask.samMask, aMask.opencv_mask);
    printf("\tcompute_mask_center...\n");
    std::vector<cv::Vec4i> hierarchy;
    printf("\t\tfindContours...\n");
    cv:findContours(aMask.opencv_mask, aMask.contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE );
    if (aMask.contours.size() == 0) 
        printf("WARNING: No countours found for the mask!\n");
    
    int biggest_contour_index = 0;
    int biggest_contour_area = 0;
    for (int i=0; i<aMask.contours.size(); i++) {
        int contour_area = cv::contourArea(aMask.contours[i]);
        if (contour_area > biggest_contour_area) {
            biggest_contour_area = contour_area;
            biggest_contour_index = i;
        }
    }
    printf("Biggest contour is %d with area %d\n", biggest_contour_index, biggest_contour_area);

    //TODO: Multiple contours
    //printf("\t\tcontourArea...\n");
    aMask.mask_contour_size = biggest_contour_area;//TODO WHY DO WE USE THAT???
    //printf("new_contour_area = %d \n", aMask.mask_contour_size);

    // compute the center of the contour https://pyimagesearch.com/2016/02/01/opencv-center-of-contour/
    cv::Moments M = cv::moments(aMask.contours[biggest_contour_index]);
    cv::Point center(M.m10/M.m00, M.m01/M.m00);  
    aMask.mask_center_x = center.x;
    aMask.mask_center_y = center.y;
    //circle(aMask.mask, center, 5, cv::Scalar(128,0,0), -1);//DEBUG
} 

//called by propagate_masks (called by editor and cli_masks)
//but the editor also calls compute_mask_and_textures in sam_utils.cpp
bool compute_mask(Mask & aMask, sam_image_u8 img0, Segmentor & segmentor) {

    //Compute the frame: Obtain the best mask at the point
    //cv::Mat output = get_best_opencv_mask_at_point(aMask.mask_computed_at_x, aMask.mask_computed_at_y, img0, state, n_threads);
    
    //compute mask at given point (pick best one)
    //bool found = get_best_sam_mask_at_point(aMask.mask_computed_at_x, aMask.mask_computed_at_y, img0, state, n_threads, aMask.samMask);
    
    //one point version:
    //bool found = segmentor.get_best_mask_at_point(aMask.mask_computed_at_x, aMask.mask_computed_at_y, img0, aMask.samMask); 
    //multiple points
    bool found = segmentor.get_best_mask_at_points(aMask.mask_computed_at_points, img0, aMask.samMask); 

    if (found) {
        compute_mask_center(aMask);
        aMask.mask_computed = true;
    }
    return found;
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
int propagate_masks_naive(std::vector<Frame> & frames, Segmentor & segmentor, int from_frame, int till_frame, float & progress, bool & cancel) 
{
    printf("propagating from %d (reference) to %d\n", from_frame, till_frame);
    //int MAX = 5;
    progress = 0;
    int numFrames = frames.size();
    int f = 0;
    //iterate through all frames 
    //assumes that the first frame has been already computed
    for (Frame & aFrame : frames) {
        if (cancel) {
            printf("propagation cancelled\n");
            progress = 1;
            cancel = false;
            break;
        }
        if (f > till_frame) {
            break;
        }
        printf("PROCESSING FRAME %d \n", f);
        
        //1) PREPROCESS FRAME AND COMPUTE FACIAL TRAINTS (all frames except the first one)
        //to do at frame level (not at mask level)
        //only if not the first frame: precompute the image
        //only if not the first frame: compute faces
        if (f>from_frame) {
            printf("\t%d>from_frame so precomputing. \n", f);
            segmentor.preprocessImage(aFrame.img_sam_format);
            //compute faces, but only if weren't computed before
            //does not make sense to recompute faces
            //TODO: What happens if color changes?
            if (aFrame.faces_check && !aFrame.faces_computed) {
                printf("Computing face...\n");
                cv::Mat face_mask = cv::Mat(cv::Size(aFrame.img.cols,aFrame.img.rows), CV_8UC4, cv::Scalar(0,0,0,0));
                cv::Mat eyes_mask = cv::Mat(cv::Size(aFrame.img.cols,aFrame.img.rows), CV_8UC4, cv::Scalar(0,0,0,0));
                //TODO
                //cv::Scalar faceColor = cv::Scalar(myState.face_color[0]*255, myState.face_color[1]*255, myState.face_color[2]*255, myState.face_color[3]*255);
                //cv::Scalar pupilsColor = cv::Scalar(myState.eyes_color[0]*255, myState.eyes_color[1]*255, myState.eyes_color[2]*255, myState.eyes_color[3]*255);
                //NOTE: Currently the face mask is treated as a binary mask and these colors are ignored
                //      The colors are applied in the editor when rendering the binary texture (draw_list->AddImage)
                cv::Scalar faceColor = cv::Scalar(168, 113, 118, 255);
                cv::Scalar pupilsColor = cv::Scalar(118, 71, 61, 255);
                face(aFrame.img, face_mask, eyes_mask, faceColor, pupilsColor);


                //cv::Mat blankImageWithAlpha = cv::Mat(cv::Size(aFrame.img.cols,aFrame.img.rows), CV_8UC4, cv::Scalar(0,0,0,0));
                //face(aFrame.img, blankImageWithAlpha, cv::Scalar(118, 113, 168, 255), cv::Scalar(61, 71, 118, 255));
                //aFrame.faces = blankImageWithAlpha;
                aFrame.faces = face_mask;
                aFrame.eyes = eyes_mask;

                //GLuint newGLTextureFace = createGLTextureOpenCV(blankImageWithAlpha, GL_RGBA);
                //aFrame.facesTexture = newGLTextureFace;
                aFrame.faces_computed = true;
                printf("Face computed.\n");
            }

        } 
        //2) COPY ALL MASKS PARAMS (all frames) AND COMPUTE ALL MASKS (all except the first one)

        //to do at mask level 
        //all frames, including the first
        //the first one to copy mask parameters to next ones
        //if not the first one to actually compute each mask
        if (f>=from_frame) {
            printf("\t%d>=from_frame so analyzing masks. \n", f);
            //If need to propagate to next frame let's clear it's masks
            if (f < till_frame && f < numFrames-1) {
                frames[f+1].masks.clear();
                if (aFrame.faces_check) //if this frame has faces, check the next one
                    frames[f+1].faces_check = true;
            } 
            //printf("\t#Masks in frame %d = %d. \n", f, aFrame.masks.size());
            

            std::vector<int> notFoundMasksIds;
            //iterate through all the masks of the frame
            int m = 0;
            for (Mask & aMask : aFrame.masks) {
                printf("\tPROCESSING MASK %d \n", aMask.maskId);
                printf("\taMask.mask_computed_at_x %d \n", aMask.mask_computed_at_x);
                printf("\taMask.mask_computed_at_y %d \n", aMask.mask_computed_at_y);
                
                //COMPUTE MASK (if not the first frame)
                bool found = false; //the one in the reference frame
                if (f>from_frame) { 
                    printf("\t%d>from_frame so computing mask. \n", f);
                    //if the previous failed do not keep computing 
                    if (frames[f-1].getMaskById(aMask.maskId)->mask_center_x != -1)
                        found = compute_mask(aMask, aFrame.img_sam_format, segmentor);
                    progress = (f-from_frame+1)/(float)(till_frame-from_frame+1);
                    //printf("progress=%f\n", progress);
                }
                if (!found) {
                    //Instead of removing it, it will remain as mask_computed=false
                    /*
                    //Do not delete, it works. 
                    //Removes masks if not found
                    printf("\tRemoving mask with id %d because not found...\n", aMask.maskId);
                    notFoundMasksIds.push_back(aMask.maskId);
                    if (aFrame.removeMaskById(aMask.maskId)) {
                        printf("\tRemoved mask with id %d.\n", aMask.maskId);
                    } else {
                        printf("\tWARNING: Not removed mask with id %d.\n", aMask.maskId);
                    }*/
                } else {
                    printf("\tMask found\n", aMask.maskId);
                }
                //compute mask center
                //compute_mask_center(aMask, aFrame.img_sam_format, state, n_threads);
                
                //COPY MASK TO NEXT FRAME
                //if not the last frame
                //add the mask to the next frame with the next coordinates
                //if the mask is not found it's not added to the following frame
                printf("\tf=%d numFrames=%d.\n", f, numFrames);
                if (f < till_frame && f < numFrames-1) {// && found) {
                    Mask newMask;
                    newMask.maskId = aMask.maskId;
                    newMask.color[0] = aMask.color[0];
                    newMask.color[1] = aMask.color[1];
                    newMask.color[2] = aMask.color[2];
                    newMask.track_movement = aMask.track_movement;
                    
                    bool repositioned = false;
                    if (newMask.track_movement && found) {
                        //the next mask will not be computed at the same point
                        //it will be computed at the center of this mask
                        //newMask.mask_computed_at_x = aMask.mask_center_x;
                        //newMask.mask_computed_at_y = aMask.mask_center_y; 

                        //if there's a previous mask we can use this and previous
                        //to compute the displacement of the centers
                        if (f>0) {
                            Mask *previousMask = frames[f-1].getMaskById(aMask.maskId);
                            if (previousMask != nullptr && found) {
                                printf("Recalculating mask pos.\n");
                                printf("\t previous mask center: %d,%d.\n", previousMask->mask_center_x, previousMask->mask_center_y);
                                printf("\t current mask center: %d,%d.\n", aMask.mask_center_x, aMask.mask_center_y);
                                newMask.mask_computed_at_x = aMask.mask_computed_at_x+aMask.mask_center_x-previousMask->mask_center_x;
                                printf("\t newMask.mask_computed_at_x = %d.\n", newMask.mask_computed_at_x);
                                
                                newMask.mask_computed_at_y = aMask.mask_computed_at_y+aMask.mask_center_y-previousMask->mask_center_y;
                                printf("\t newMask.mask_computed_at_y = %d.\n", newMask.mask_computed_at_y);

                                //TODO: Is this a deep copy or not??
                                newMask.mask_computed_at_points = aMask.mask_computed_at_points;
                                for (int p=0; p<newMask.mask_computed_at_points.size(); p++) {
                                    newMask.mask_computed_at_points[p].x =  aMask.mask_computed_at_points[p].x+aMask.mask_center_x-previousMask->mask_center_x;
                                    newMask.mask_computed_at_points[p].y =  aMask.mask_computed_at_points[p].y+aMask.mask_center_y-previousMask->mask_center_y;
                                    //printf("\t newMask.mask_computed_at_points[p].x = %d.\n", newMask.mask_computed_at_points[p].x);
                                }

                                repositioned = true;
                            } else {
                                printf("Cannot recalculate mask pos cause not found in previous frame\n");
                            }
                        } 

                    }
                    //COPY MASK COORDS WITHOUT CHANGES
                    if (!newMask.track_movement || !repositioned) {
                        //NOTE: Will always enter here for the first frame as repositioned will be false
                        printf("No repositioned because newMask.track_movement == %d or repositioned= %d\n", newMask.track_movement, repositioned); 
                        //copy the same pos to next
                        newMask.mask_computed_at_x = aMask.mask_computed_at_x;
                        newMask.mask_computed_at_y = aMask.mask_computed_at_y; 
                        newMask.mask_computed_at_points = aMask.mask_computed_at_points;
                    }
                    //newMask.mask_computed_at_x = aMask.mask_center_x;
                    //newMask.mask_computed_at_y = aMask.mask_center_y; 
                    printf("\tADDING MASK %d TO FRAME %d WITH x=%d, y=%d\n", aMask.maskId, f, newMask.mask_computed_at_x, newMask.mask_computed_at_y);
                    frames[f+1].masks.push_back(newMask);
                }
                m++;
            }
            printf("\tMASKS DONE.\n");

            //remove not found masks
            /*for (int & aMaskId: notFoundMasksIds) {
                printf("Removing mask %d because not found...\n", aMaskId);
                frames[f].masks.erase(frames[f].masks.begin() + aMaskIdx);
                printf("Removed.\n", aMaskId);
            }*/
            /*for (int & aMaskId: notFoundMasksIds) {
                printf("Removing mask with id %d because not found...\n", aMaskId);
                if (removeMaskById(aMaskId)) {
                    printf("Removed mask with id %d.\n", aMaskId);
                } else {
                    printf("WARNING: Not removed mask with id %d.\n", aMaskId);
                }
            }*/
        } else {
            printf("\t%d<from_frame so NOT analyzing masks. \n", f);
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

//We assume the first frame has already the masks and the user coordinates
//and also the computation 
//frames are in memory
//ONGOING
int propagate_masks(std::vector<Frame> & frames, Segmentor & segmentor, int from_frame, int till_frame, float & progress, bool & cancel) 
{
    /*
    for (Frame & aFrame : frames) {
        printf("\tPreprocessing frame... \n");
        segmentor.preprocessImage_and_remember(aFrame.img_sam_format);
    }
    */

    int numFrames = frames.size();

    //I need to keep all the embeddings first.
    Frame firstFrame = frames[from_frame];

    //iterate through all the masks
    int m = 0;
    for (Mask & aMask : firstFrame.masks) {

        //Each mask is a complete new run of the segmentor
        segmentor.reset_memory();
        printf("\tPROCESSING MASK %d \n", aMask.maskId);
        int f = 0;
        for (Frame & aFrame : frames) {
            if (cancel) {
                printf("propagation cancelled\n");
                progress = 1;
                cancel = false;
                break;
            }
            if (f > till_frame) {
                break;
            }
            progress = (f-from_frame+1)/(float)(till_frame-from_frame+1);
            
            //TODO replace by preprocessImage_and_remember
            segmentor.preprocessImage(aFrame.img_sam_format);

            Mask *currentMask = aFrame.getMaskById(aMask.maskId);


            bool found = segmentor.get_best_mask_at_points_video(currentMask->mask_computed_at_points, aFrame.img_sam_format, currentMask->samMask, f); 

            if (found && f < till_frame && f < numFrames-1) {
                currentMask->mask_computed = true;

                //COPY NO NEXT FRAME:
                Mask newMask;
                newMask.maskId = currentMask->maskId;
                newMask.color[0] = currentMask->color[0];
                newMask.color[1] = currentMask->color[1];
                newMask.color[2] = currentMask->color[2];
                //newMask.track_movement = aMask.track_movement;
                newMask.mask_computed_at_x = currentMask->mask_computed_at_x;
                newMask.mask_computed_at_y = currentMask->mask_computed_at_y; 
                newMask.mask_computed_at_points = currentMask->mask_computed_at_points;
                printf("\tADDING MASK %d TO FRAME %d WITH x=%d, y=%d\n", currentMask->maskId, f, newMask.mask_computed_at_x, newMask.mask_computed_at_y);
                frames[f+1].masks.push_back(newMask);
            } else 
                break;
            f++;
        }
        m++;
    }
	return 0;
            
}

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
} 
