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
    cv::resize(opencv_image, opencv_image, inputSize);
    bool successPreprocessImage = sam.preprocessImage(opencv_image);
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

cv::Mat SAM2Segmentor::get_best_mask_at_point(int x, int y, cv::Mat& image_opencv) {

    cv::Size imageSize = cv::Size(image_opencv.cols, image_opencv.rows);
    std::list<cv::Rect> rects;
    std::list<cv::Point> points;
    std::vector<float> inputPointValues, inputLabelValues;
    int previousMaskIdx = -1;
    bool isNextGetMask = true;
    cv::Mat mask;
    cv::Size inputSize = sam.getInputSize();
    cv::Rect rect1 = cv::Rect(1215 * inputSize.width / imageSize.width,
                            125 * inputSize.height / imageSize.height,
                            508 * inputSize.width / imageSize.width,
                            436 * inputSize.height / imageSize.height);
    cv::Rect rect2 = cv::Rect(890 * inputSize.width / imageSize.width,
                            85 * inputSize.height / imageSize.height,
                            315 * inputSize.width / imageSize.width,
                            460 * inputSize.height / imageSize.height);
    rects.push_back(rect1);
    rects.push_back(rect2);
    sam.setRectsLabels(rects, &inputPointValues, &inputLabelValues);
    int batchNum = (int)rects.size();
    mask = sam.getMaskBatch(inputPointValues, inputLabelValues, batchNum, imageSize);
    cv::imwrite("mask_box_batch.png", mask);
    inputPointValues.resize(0);//delete
    inputLabelValues.resize(0);//delete
    rects.resize(0);//delete

    rects.push_back(rect1);
    sam.setRectsLabels(rects, &inputPointValues, &inputLabelValues);
    mask = sam.getMask(inputPointValues, inputLabelValues, imageSize, previousMaskIdx, isNextGetMask);
    previousMaskIdx++;
    cv::imwrite("mask_box1.png", mask);
    inputPointValues.resize(0);
    inputLabelValues.resize(0);
    rects.resize(0);
    

    //opencv image to sam
    /*sam_image_u8 image_sam;
    opencv_image2sam(image_sam, image_opencv);

    //compute mask at given point (pick best one)
    sam_image_u8 mask;
    bool found = get_best_sam_mask_at_point(x, y, image_sam, *a_sam_state, params.n_threads, mask);
    */
    //convert mask to opencv format
    cv::Mat mask_opencv;
    //sam_image_grayscale2opencv(mask, mask_opencv);
    //TODO
    return mask_opencv;
}

void SAM2Segmentor::close() {
    //TODO
    //sam_deinit(*a_sam_state);
}
