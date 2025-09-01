#include <opencv2/opencv.hpp>
#define STRIP_FLAG_HELP 1
#ifdef _WIN32 //WIN32
	//
#else
	#include <gflags/gflags.h>
#endif
#include <thread>
#include "segmentor.h"
#include "segmentor_sam21.h"
#include "common1.h"
//#include "sam21.h"

/*

    IMPORTANT: To run (on Macos) change SAM21Segmentor::SAM21Segmentor() 
    in segmentor_sam21.cpp
    to: sam21(SAM21(modelsPath(false), "t"))

*/

inline cv::Mat overlayMask(const cv::Mat& image, const cv::Mat& maskProb, 
                    const cv::Scalar& color = cv::Scalar(0,0,255), // red (BGR)
                    double alpha = 0.5) 
{
    CV_Assert(image.type() == CV_8UC3);
    CV_Assert(maskProb.type() == CV_32FC1 || maskProb.type() == CV_8UC1);
    CV_Assert(image.size() == maskProb.size());

    // threshold if needed
    cv::Mat mask;
    if (maskProb.type() == CV_32FC1) {
        cv::threshold(maskProb, mask, 0.5, 1.0, cv::THRESH_BINARY);
        mask.convertTo(mask, CV_8UC1, 255.0); // 0 or 255
    } else {
        mask = maskProb.clone();
        if (mask.depth() != CV_8U) mask.convertTo(mask, CV_8UC1);
    }

    // make a 3-channel color mask
    cv::Mat maskColor(image.size(), CV_8UC3, color);
    
    // apply mask to color
    cv::Mat colored;
    maskColor.copyTo(colored, mask);

    // blend
    cv::Mat blended;
    cv::addWeighted(image, 1.0, colored, alpha, 0.0, blended);

    return blended;
}

void test_without_segmentor_parent() {
    printf("Creating models...\n");
    SAM21 sam21 = SAM21("checkpoints/ailia_sam21_tiny", "t");
    printf("calling sam21.test()\n");
    sam21.test();
}

void test_single() {
    ///////////////////////
    // test with Segmentor (1 image)
    ///////////////////////
    Segmentor& s21 = get_sam21_segmentor();  
    //s21.test();
    std::string image_path2 = "/Users/rtous/dev/back2black/img4.png";
    cv::Mat image_opencv2 = imread(image_path2, cv::IMREAD_COLOR);

    s21.preprocessImage(image_opencv2);


    //Click #1
    
    std::vector<MaskPoint> mask_points;//600, 450
    MaskPoint p;
    p.x = 485;//600
    p.y = 355;//450
    p.positive = true;
    mask_points.push_back(p);

    sam_image_u8 mask_sam;
    sam_image_u8 sam_image;
    opencv_image2sam(sam_image, image_opencv2);
    s21.get_best_mask_at_points(mask_points, sam_image, mask_sam);
    //sam_image_u8 mask_rgba_binary = sam_mask_to_sam_4channels(mask_sam, 180);
    cv::Mat res;
    sam_image_grayscale2opencv(mask_sam, res);

    //cv::Mat overlayed = overlayMask(image_opencv2, res, cv::Scalar(255,0,0), 0.4);

    cv::imwrite("/Users/rtous/dev/back2black/borrar_sam21_click1.png", res);  

     //Click #2

    mask_points.clear();
    p.x = 485;
    p.y = 355;
    p.positive = true;
    mask_points.push_back(p);

    mask_sam;
    sam_image;
    opencv_image2sam(sam_image, image_opencv2);
    s21.get_best_mask_at_points(mask_points, sam_image, mask_sam);
    //sam_image_u8 mask_rgba_binary = sam_mask_to_sam_4channels(mask_sam, 180);
    res;
    sam_image_grayscale2opencv(mask_sam, res);

    //cv::Mat overlayed = overlayMask(image_opencv2, res, cv::Scalar(255,0,0), 0.4);

    cv::imwrite("/Users/rtous/dev/back2black/borrar_sam21_click2.png", res); 

}
void test_video_without_presaving_all_embeddings() {
    Segmentor& s21 = get_sam21_segmentor();  

    std::vector<MaskPoint> mask_points;//600, 450
    MaskPoint p;
    p.x = 485;//600
    p.y = 355;//450
    p.positive = true;
    mask_points.push_back(p);

    sam_image_u8 mask_sam;
    sam_image_u8 sam_image;

    ///////////////////////
    // test with Segmentor (video without presaving all embeddings)
    ///////////////////////

    s21.reset_memory();

    printf("Reading video...\n");
    std::string video_path = "footage.mp4";
    cv::VideoCapture capture(video_path);
    if (!capture.isOpened()) exit(0);

    mask_points.clear();
    p.x = 485;//600
    p.y = 355;//450
    p.positive = true;
    mask_points.push_back(p);

    cv::Mat frame;
    int i = 0;
    while (true) {
      printf("\n**************************\n");
      printf("Frame %d\n", i);
      printf("**************************\n");
      if (!capture.read(frame) || frame.empty()) break;
      printf("Inferencing frame...\n");
      s21.preprocessImage(frame);

      printf("Inferencing frame...\n");
      mask_sam;
      sam_image;
      opencv_image2sam(sam_image, frame);

      s21.get_best_mask_at_points_video(mask_points, sam_image, mask_sam, i);
      //sam_image_u8 mask_rgba_binary = sam_mask_to_sam_4channels(mask_sam, 180);
      cv::Mat res;  
      sam_image_grayscale2opencv(mask_sam, res);
      printf("mask_opencv size: %d x %d\n", res.cols, res.rows);
      printf("mask_sam nx=%d ny=%d data=%zu\n", mask_sam.nx, mask_sam.ny, mask_sam.data.size());
      //inference_frame(frame, i, memory_info, img_encoder, prompt_encoder, img_decoder, mem_encoder, mem_attention, mlp, obj_ptr_tpos_proj_hiera, inference_state, promptPoints);
      
      //cv::imshow("frame", res);
      //cv::waitKey(0);

      cv::Mat overlayed = overlayMask(frame, res, cv::Scalar(0,255,0), 0.4);
      std::ostringstream ss;
      cv::imwrite("output/debug_frame_"+std::to_string(i)+"_.png", overlayed);

      i++;
      printf("Frame 1 DONE.\n");
    }
    capture.release(); 
}
void test_video_presaving_all_embeddings() {
    /*
    Segmentor& s21 = get_sam21_segmentor();  

    std::vector<MaskPoint> mask_points;//600, 450
    MaskPoint p;
    p.x = 485;//600
    p.y = 355;//450
    p.positive = true;
    mask_points.push_back(p);

    sam_image_u8 mask_sam;
    sam_image_u8 sam_image;

    ///////////////////////
    // test with Segmentor (video presaving all embeddings)
    ///////////////////////

    s21.reset_memory();

    printf("Reading video...\n");
    std::string video_path = "footage.mp4";
    cv::VideoCapture capture(video_path);
    if (!capture.isOpened()) return 0;

    mask_points.clear();
    p.x = 485;//600
    p.y = 355;//450
    p.positive = true;
    mask_points.push_back(p);

    std::vector<cv::Mat> frames;
    cv::Mat frame;
    int i = 0;
    while (true) {
      printf("\n**************************\n");
      printf("Frame %d\n", i);
      printf("**************************\n");
      if (!capture.read(frame) || frame.empty()) break;
      printf("Inferencing frame...\n");
      s21.preprocessImage_and_remember(frame);
      frames.push_back(frame);
      i++;
      printf("Frame preprocessed.\n");
    }
    capture.release();

    i = 0;
    while (true) {
      printf("\n**************************\n");
      printf("Frame %d\n", i);
      printf("**************************\n");
      
      printf("Inferencing frame...\n");
      mask_sam;
      sam_image;
      opencv_image2sam(sam_image, frames[i]);
      s21.get_best_mask_at_points_video(mask_points, sam_image, mask_sam, i);
      //sam_image_u8 mask_rgba_binary = sam_mask_to_sam_4channels(mask_sam, 180);
      cv::Mat res;  
      sam_image_grayscale2opencv(mask_sam, res);
      //inference_frame(frame, i, memory_info, img_encoder, prompt_encoder, img_decoder, mem_encoder, mem_attention, mlp, obj_ptr_tpos_proj_hiera, inference_state, promptPoints);
      cv::imshow("frame", res);
      cv::waitKey(0);
      i++;
      printf("Frame 1 DONE.\n");
    }
    capture.release();
    */

    //s21.close();
}

int main(int argc, char** argv) {
    printf("calling test_without_segmentor_parent()...\n");
	test_without_segmentor_parent();
    //test_video_without_presaving_all_embeddings();
}
