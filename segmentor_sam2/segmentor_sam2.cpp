#include "stdio.h"
#include "segmentor_sam2.h"
#include "common1.h"

Segmentor& get_sam2_segmentor() 
{
    static SAM2Segmentor s2;
    return s2;
}

SAM2Segmentor::SAM2Segmentor() 
{
    printf("called SAM2Segmentor::SAM2Segmentor()\n");
    //TODO
    sam.changeMode(SAM2);
    bool successLoadModel = sam.loadModel("checkpoints/sam2.1_tiny/sam2.1_tiny_preprocess.onnx", "checkpoints/sam2.1_tiny/sam2.1_tiny.onnx", std::thread::hardware_concurrency(), "cpu");
    if(!successLoadModel){
        std::cout<<"loadModel error"<<std::endl;
        exit(1);
    }
    printf("SAM 2 setup finished\n");
};

void SAM2Segmentor::test() 
{
    printf("called SAM2Segmentor::test()\n");
};

bool SAM2Segmentor::preprocessImage(cv::Mat & opencv_image) {
    printf("SAM2Segmentor::preprocessImage()\n");
    cv::Size imageSize = cv::Size(opencv_image.cols, opencv_image.rows);
    cv::Size inputSize = sam.getInputSize();
    cv::Mat opencv_image_resized;
    cv::resize(opencv_image, opencv_image_resized, inputSize);
    bool successPreprocessImage = sam.preprocessImage(opencv_image_resized);
    if(!successPreprocessImage){
        std::cout<<"preprocessImage error"<<std::endl;
        return false;
    }

    /*sam_image_u8 sam_image;
    opencv_image2sam(sam_image, opencv_image);
    if (!sam_compute_embd_img(sam_image, params.n_threads, *a_sam_state)) {
        fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
        return false;
    }*/
    //TODO
    return true;
}

bool SAM2Segmentor::preprocessImage(sam_image_u8 & image_sam) {
    cv::Mat image_opencv;
    sam_image_color2opencv(image_sam, image_opencv);
    return preprocessImage(image_opencv);
}

cv::Mat SAM2Segmentor::get_best_mask_at_point(int x, int y, cv::Mat& image_opencv) {
    cv::Size imageSize = cv::Size(image_opencv.cols, image_opencv.rows);
    //std::list<cv::Rect> rects;
    std::list<cv::Point> points;
    std::vector<float> inputPointValues, inputLabelValues;
    int previousMaskIdx = -1;
    bool isNextGetMask = true;
    cv::Mat mask_opencv;
    cv::Size inputSize = sam.getInputSize();

    cv::Point point1 = cv::Point(x * inputSize.width / imageSize.width,
                                 y * inputSize.height / imageSize.height);
    //cv::Point point2 = cv::Point(1500 * inputSize.width / imageSize.width,
    //                               420 * inputSize.height / imageSize.height);
    points.push_back(point1);
    //points.push_back(point2);
    sam.setPointsLabels(points, 1, &inputPointValues, &inputLabelValues);
    printf("sam.getMask...\n");
    mask_opencv = sam.getMask(inputPointValues, inputLabelValues, imageSize, previousMaskIdx, isNextGetMask);
    //cv::resize(mask_opencv, mask_opencv, imageSize);
    printf("sam.getMask DONE\n");
    return mask_opencv;
}

bool SAM2Segmentor::get_best_mask_at_point(int x, int y, sam_image_u8& image_sam, sam_image_u8& mask_sam) {
    cv::Mat image_opencv;
    sam_image_color2opencv(image_sam, image_opencv);
    cv::Mat mask_opencv = get_best_mask_at_point(x, y, image_opencv);
    opencv_image2sam_binarymask(mask_sam, mask_opencv);
    return true;//TODO
}

void SAM2Segmentor::close() {
    //TODO
    //sam_deinit(*a_sam_state);
}
