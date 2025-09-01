#include "stdio.h"
#include "segmentor_sam21.h"
#include "sam21.h"
#include "common1.h"
#include <filesystem>
#ifdef __APPLE__
    #include "CoreFoundation/CoreFoundation.h"
    #include <unistd.h>
    #include <libgen.h>
#endif

namespace fs = std::filesystem;

Segmentor& get_sam21_segmentor() 
{
	printf("get_sam21_segmentor()\n");
    static SAM21Segmentor s21;
    return s21;
}

void debug_opencv_image(cv::Mat& image) {
    int depth = image.depth();
    int channels = image.channels();
    std::string depthStr;
    switch (depth) {
        case CV_8U:  depthStr = "CV_8U"; break;
        case CV_8S:  depthStr = "CV_8S"; break;
        case CV_16U: depthStr = "CV_16U"; break;
        case CV_16S: depthStr = "CV_16S"; break;
        case CV_32S: depthStr = "CV_32S"; break;
        case CV_32F: depthStr = "CV_32F"; break;
        case CV_64F: depthStr = "CV_64F"; break;
        default:     depthStr = "Unknown"; break;     
    }
    std::cout << "depth: " << depthStr << " with " 
          << channels << " channel(s)" << std::endl;  
    for (int row=0; row < image.rows; ++row){
        if (row==1) printf("\n");
        for (int col=0; col < image.cols; ++col){
            if (row<=1 and col<10) {
                switch (depth) {
                    case CV_8U:  std::cout << (int)image.at<cv::Vec<unsigned char, 1>>(row, col)[0] << " ";; break;
                    case CV_8S:  std::cout << (int)image.at<cv::Vec<char, 1>>(row, col)[0] << " ";; break;
                    case CV_16U: std::cout << image.at<cv::Vec<unsigned short, 1>>(row, col)[0] << " ";; break;
                    case CV_16S: std::cout << image.at<cv::Vec<short, 1>>(row, col)[0] << " ";; break;
                    case CV_32S: std::cout << image.at<cv::Vec<int, 1>>(row, col)[0] << " ";; break;
                    case CV_32F: std::cout << image.at<cv::Vec<float, 1>>(row, col)[0] << " "; break;
                    case CV_64F: std::cout << image.at<cv::Vec<double, 1>>(row, col)[0] << " "; break;
                    default:     printf("UNKNOWN depth\n"); break;
                }
                
            }
        }
        //printf("\n");
    }      
}

/*
inline cv::Mat overlayMaskBinaryU8(const cv::Mat& image,
                                   const cv::Mat& mask,
                                   const cv::Scalar& color = cv::Scalar(0, 255, 0), // green
                                   double alpha = 0.5) 
{
    CV_Assert(image.type() == CV_8UC3);
    CV_Assert(mask.type() == CV_8UC1);
    CV_Assert(image.size() == mask.size());

    // Create solid color image
    cv::Mat maskColor(image.size(), CV_8UC3, color);

    // Apply binary mask: only keep "color" where mask==255
    cv::Mat colored;
    maskColor.copyTo(colored, mask);  // mask acts as selector

    // Blend with original image
    cv::Mat blended;
    cv::addWeighted(image, 1.0, colored, alpha, 0.0, blended);

    return blended;
}
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

static std::string modelsPath(bool release) {
        printf("called SAM21Segmentor::SAM21Segmentor()\n");
        std::string models_absolutepath;
        std::string models_relativepath = "checkpoints/ailia_sam21_tiny";

        if (release) {
            #ifdef __APPLE__
                printf("APPLE detected, inspecting bundle path...\n");
                CFBundleRef bundle = CFBundleGetMainBundle();
                CFURLRef bundleURL = CFBundleCopyBundleURL(bundle);
                char path[PATH_MAX];
                Boolean success = CFURLGetFileSystemRepresentation(bundleURL, TRUE, (UInt8 *)path, PATH_MAX);
                assert(success);
                CFRelease(bundleURL);
                printf(path);
                models_absolutepath = std::string(path)+"/Contents/Resources/"+models_relativepath;
            #else
                printf("APPLE NOT detected\n");
                models_absolutepath = models_relativepath;
            #endif

            if (!fs::exists(models_absolutepath)) {
                printf("ERROR: SAM 2.1 models path wrong (setup in segmentor_sam21.cpp): %s\n", models_absolutepath.c_str());
            }
        } else {
            models_absolutepath = models_relativepath;
        }
		printf("models_absolutepath = %s\n", models_absolutepath.c_str());
		
		if (!fs::exists(models_absolutepath))
                printf("ERROR: SAM 2.1 models path wrong (setup in segmentor_sam21.cpp): %s\n", models_absolutepath.c_str());
		else
				printf("Model folder exists.\n");
        return models_absolutepath;
}

SAM21Segmentor::SAM21Segmentor() :
    sam21(SAM21(modelsPath(false), "t"))//The param should be true except when debugging from a no bundle version
{
    printf("SAM 2 setup finished\n");
};

void SAM21Segmentor::test() 
{
    printf("called SAM21Segmentor::test()\n");
    sam21.test();
};

bool SAM21Segmentor::preprocessImage(cv::Mat & opencv_image) {
    printf("SAM21Segmentor::preprocessImage() (with opencv_image)\n");
    sam21.image_encoder(opencv_image);
    return true;
}

bool SAM21Segmentor::preprocessImage(sam_image_u8 & image_sam) {
    printf("SAM21Segmentor::preprocessImage() (with sam_image_u8)\n");
    /*
    cv::Mat image_opencv;
    sam_image_color2opencv(image_sam, image_opencv);
    return preprocessImage(image_opencv);
    */
    cv::Mat image_opencv;
    sam_image_color2opencv(image_sam, image_opencv);
    return preprocessImage(image_opencv);
}

bool SAM21Segmentor::preprocessImage_and_remember(cv::Mat & opencv_image) {
    printf("SAM21Segmentor::preprocessImage() (with opencv_image)\n");
    sam21.image_encoder(opencv_image);
    //add to video_frames_embeddings
    FrameEmbedding fe;
    fe.vision_features = sam21.inference_state.vision_features;
    fe.backbone_fpn_0  = sam21.inference_state.backbone_fpn_0;
    fe.backbone_fpn_1  = sam21.inference_state.backbone_fpn_1;
    fe.dense_pe        = sam21.inference_state.dense_pe;
    sam21.video_frames_embeddings.push_back(fe);
    return true;
}

bool SAM21Segmentor::preprocessImage_and_remember(sam_image_u8 & image_sam) {
    printf("SAM21Segmentor::preprocessImage() (with sam_image_u8)\n");
    /*
    cv::Mat image_opencv;
    sam_image_color2opencv(image_sam, image_opencv);
    return preprocessImage(image_opencv);
    */
    cv::Mat image_opencv;
    sam_image_color2opencv(image_sam, image_opencv);
    return preprocessImage(image_opencv);
}

//es pot eliminar:
//one point, image opencv format
cv::Mat SAM21Segmentor::get_best_mask_at_point(int x, int y, cv::Mat& image_opencv) {
    cv::Mat mask_opencv;
	/*
    std::vector<MaskPoint> mask_points;
    MaskPoint mask_point;
    mask_point.x = x;
    mask_point.y = y;
    mask_point.positive = true;
    mask_points.push_back(mask_point);
    return get_best_mask_at_points(mask_points, image_opencv);
    */
	return mask_opencv;
}

//multiple points, image opencv format
cv::Mat SAM21Segmentor::get_best_mask_at_points(std::vector<MaskPoint>& mask_points, cv::Mat& image_opencv) {
    printf("get_best_mask_at_points\n");
    cv::Mat mask_opencv;
    std::vector<PromptPoint> promptPoints;
    for (int i = 0; i < mask_points.size(); i++) {
        //std::list<cv::Point> points;
        MaskPoint mask_point = mask_points[i];
        PromptPoint pp; 
        pp.x = mask_point.x;
        pp.y = mask_point.y;
        if (mask_point.positive)
            pp.label = 1;
        else 
            pp.label = 0; 
        promptPoints.push_back(pp);
    }
    //sam21.image_encoder(image_opencv);

    mask_opencv = sam21.inference_frame(image_opencv, 0, promptPoints, false, false);
    //cv::imwrite("/Users/rtous/dev/back2black/borrar_sam21_.png", mask_opencv);   
    return mask_opencv;
}

//es pot eliminar:
//one point, image sam format
bool SAM21Segmentor::get_best_mask_at_point(int x, int y, sam_image_u8& image_sam, sam_image_u8& mask_sam) {
    /*
    std::vector<MaskPoint> mask_points;
    MaskPoint mask_point;
    mask_point.x = x;
    mask_point.y = y;
    mask_point.positive = true;
    mask_points.push_back(mask_point);
    return get_best_mask_at_points(mask_points, image_sam, mask_sam);
    */
	return true;
}

//multiple points, image sam format
bool SAM21Segmentor::get_best_mask_at_points(std::vector<MaskPoint>& points, sam_image_u8& image_sam, sam_image_u8& mask_sam) {
    cv::Mat image_opencv;
    sam_image_color2opencv(image_sam, image_opencv);
    cv::Mat mask_opencv = get_best_mask_at_points(points, image_opencv);
    opencv_image2sam_binarymask(mask_sam, mask_opencv);
    return true;//TODO
}

bool SAM21Segmentor::get_best_mask_at_points_video(std::vector<MaskPoint>& mask_points, sam_image_u8& image_sam, sam_image_u8& mask_sam, int frame_idx) {
    printf("get_best_mask_at_points\n");
    cv::Mat mask_opencv;

    std::vector<PromptPoint> promptPoints;
    for (int i = 0; i < mask_points.size(); i++) {
        //std::list<cv::Point> points;
        MaskPoint mask_point = mask_points[i];
        PromptPoint pp; 
        pp.x = mask_point.x;
        pp.y = mask_point.y;
        if (mask_point.positive)
            pp.label = 1;
        else 
            pp.label = 0; 
        promptPoints.push_back(pp);
    }
    //sam21.image_encoder(image_opencv);

    cv::Mat image_opencv;
    sam_image_color2opencv(image_sam, image_opencv);

    mask_opencv = sam21.inference_frame(image_opencv, frame_idx, promptPoints, true, false);//not using saved embeddings but the last one
    //returns a mask with values 0..255 (not binary)


    //cv::Mat overlayed = overlayMask(image_opencv, mask_opencv, cv::Scalar(0,255,0), 0.4);
    //cv::imwrite("output/debug_frame_"+std::to_string(frame_idx)+"_before_opencv_image2sam_binarymask.png", overlayed);

    //cv::imwrite("/Users/rtous/dev/back2black/borrar_sam21_.png", mask_opencv);   
    
    printf("Input size: rows=%d cols=%d\n", image_opencv.rows, image_opencv.cols);
    printf("Resulting mask size: rows=%d cols=%d\n", mask_opencv.rows, mask_opencv.cols);
    if (mask_opencv.rows != image_opencv.rows || mask_opencv.cols != mask_opencv.cols) {
        printf("ERROR: mask_opencv need to be reescaled!\n");
        //cv::resize(mask_opencv, mask_opencv, cv::Size(image_opencv.rows, image_opencv.cols), 0, 0, cv::INTER_NEAREST);
        exit(-1);
    }

    printf("DEBUG mask_opencv has %d channels\n", mask_opencv.channels());
    opencv_image2sam_binarymask(mask_sam, mask_opencv);//not reversible
    //binarizes the mask (0 or 255)!

    /*//DEBUG
    debug_opencv_image(mask_opencv);
    for (int i=0; i < mask_opencv.rows; ++i){
        if (i==1) printf("\n");
        for (int j=0; j < mask_opencv.cols; ++j){
            if (i<=1 and j<10) printf("mask_opencv.at<uchar>(i, j)=%u\n", mask_opencv.at<uchar>(i, j)); //DEBUG
        }
        //printf("\n");
    }
    cv::Mat mask_opencv_reversed;
    sam_image_grayscale2opencv(mask_sam, mask_opencv_reversed);
    */
    
    /*printf("DEBUG resulting mask_opencv_reversed has %d channels\n", mask_opencv_reversed.channels());
    debug_opencv_image(mask_opencv_reversed);
    for (int i=0; i < mask_opencv_reversed.rows; ++i){
        if (i==1) printf("\n");
        for (int j=0; j < mask_opencv_reversed.cols; ++j){
            if (i<=1 and j<10) printf("mask_opencv_reversed.at<uchar>(i, j)=%u\n", mask_opencv_reversed.at<uchar>(i, j)); //DEBUG
        }
        //printf("\n");
    }*/
    ////

    //without previous overlayMask no change
    //image_opencv ok
    //values of mask_opencv_reversed ok

    //overlayed = overlayMaskBinaryU8(image_opencv, mask_opencv_reversed, cv::Scalar(0,255,0), 0.4);
    //cv::imwrite("output/debug_frame_"+std::to_string(frame_idx)+"_reversed_opencv_image2sam_binarymask.png", overlayed);


    return true;//TODO
}

void SAM21Segmentor::reset_memory() {
    sam21.reset_memory();
}


void SAM21Segmentor::close() {
    //TODO
    //sam_deinit(*a_sam_state);
}
