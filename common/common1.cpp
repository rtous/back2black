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
	given an image (in sam format) get the best mask (in opencv format at a given point 
*/
cv::Mat get_best_opencv_mask_at_point(int x, int y, sam_image_u8 img0, sam_state & state, int n_threads) {
	//Call sam to compute the mask at the point
	std::vector<sam_image_u8> masks;
	sam_point pt {(float)x, (float)y};
    masks = sam_compute_masks(img0, n_threads, pt, state);
    printf("[INFO] found %d masks\n", masks.size());
    sam_image_u8 mask = masks[0]; //the returned masks are ordered by iou and stability_score
    
    /*std::vector<GLuint> maskTextures;
    for (auto& mask : masks) {
        sam_image_u8 mask_rgb = { mask.nx, mask.ny, };
        mask_rgb.data.resize(3*mask.nx*mask.ny);
        for (int i = 0; i < mask.nx*mask.ny; ++i) {
            mask_rgb.data[3*i+0] = mask.data[i];
            mask_rgb.data[3*i+1] = mask.data[i];
            mask_rgb.data[3*i+2] = mask.data[i];
        }

        maskTextures.push_back(createGLTexture(mask_rgb, GL_RGB));
    }*/

	//convert mask to opencv format
	//Opencv is y,x (fil, col)
    cv::Mat mask_opencv = cv::Mat::zeros(mask.ny, mask.nx, CV_8UC1 );
    for (int i=0; i < mask_opencv.rows; ++i){
        for (int j=0; j < mask_opencv.cols; ++j){
            //output.at<uchar>(j, i) = mask.data[j*mask.nx+i];
            mask_opencv.at<uchar>(i, j) = mask.data[i*mask.nx+j];
        }
    }
    return mask_opencv;
}


void compute_object(Object anObject, sam_image_u8 img0, sam_state & state, int n_threads) {

    //Compute the frame: Obtain the best mask at the point
    cv::Mat output = get_best_opencv_mask_at_point(anObject.mask_computed_at_x, anObject.mask_computed_at_y, img0, state, n_threads);

    //Obtain the first contour
    //std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv:findContours(output, anObject.contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE );

    //TODO: Multiple contours

    anObject.mask_contour_size = cv::contourArea(anObject.contours[0]);
    printf("new_contour_area = %d \n", anObject.mask_contour_size);

    // compute the center of the contour https://pyimagesearch.com/2016/02/01/opencv-center-of-contour/
    cv::Moments M = cv::moments(anObject.contours[0]);
    cv::Point center(M.m10/M.m00, M.m01/M.m00);  
    anObject.mask_center_x = center.x;
    anObject.mask_center_y = center.y;
} 
