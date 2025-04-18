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

//one point, image opencv format
cv::Mat SAM2Segmentor::get_best_mask_at_point(int x, int y, cv::Mat& image_opencv) {
    /*printf("get_best_mask_at_point\n");
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
    printf("point1.x = %d\n", point1.x);
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
    */
    std::vector<MaskPoint> mask_points;
    MaskPoint mask_point;
    mask_point.x = x;
    mask_point.y = y;
    mask_point.positive = true;
    mask_points.push_back(mask_point);
    return get_best_mask_at_points(mask_points, image_opencv);
}

//multiple points, image opencv format
cv::Mat SAM2Segmentor::get_best_mask_at_points(std::vector<MaskPoint>& mask_points, cv::Mat& image_opencv) {
    printf("get_best_mask_at_points\n");
    cv::Size imageSize = cv::Size(image_opencv.cols, image_opencv.rows);
    //std::list<cv::Rect> rects;
    
    std::vector<float> inputPointValues, inputLabelValues;
    int previousMaskIdx = -1;
    bool isNextGetMask = true;
    cv::Mat mask_opencv;
    cv::Size inputSize = sam.getInputSize();

    /*
    /////
    std::list<cv::Point> points;
    cv::Point point = cv::Point(mask_points[0].x * inputSize.width / imageSize.width,
                                 mask_points[0].y * inputSize.height / imageSize.height);
    points.push_back(point);
    printf("point.x = %d\n", point.x);
    sam.setPointsLabels(points, 1, &inputPointValues, &inputLabelValues);
    //////
    */

    for (int i = 0; i < mask_points.size(); i++) {
        std::list<cv::Point> points;
        MaskPoint mask_point = mask_points[i];
        cv::Point point = cv::Point(mask_point.x * inputSize.width / imageSize.width,
                                     mask_point.y * inputSize.height / imageSize.height);
        points.push_back(point);
        if (mask_point.positive)
            sam.setPointsLabels(points, 1, &inputPointValues, &inputLabelValues);
        else
            sam.setPointsLabels(points, 0, &inputPointValues, &inputLabelValues);
    }
    //sam.setPointsLabels(points, 1, &inputPointValues, &inputLabelValues);

    //points.push_back(point2);
    //sam.setPointsLabels(points, 1, &inputPointValues, &inputLabelValues);
    printf("sam.getMask...\n");
    mask_opencv = sam.getMask(inputPointValues, inputLabelValues, imageSize, previousMaskIdx, isNextGetMask);
    //cv::resize(mask_opencv, mask_opencv, imageSize);
    printf("sam.getMask DONE\n");
    return mask_opencv;
}

//one point, image sam format
bool SAM2Segmentor::get_best_mask_at_point(int x, int y, sam_image_u8& image_sam, sam_image_u8& mask_sam) {
    std::vector<MaskPoint> mask_points;
    MaskPoint mask_point;
    mask_point.x = x;
    mask_point.y = y;
    mask_point.positive = true;
    mask_points.push_back(mask_point);
    return get_best_mask_at_points(mask_points, image_sam, mask_sam);
    /*cv::Mat image_opencv;
    sam_image_color2opencv(image_sam, image_opencv);
    cv::Mat mask_opencv = get_best_mask_at_point(x, y, image_opencv);
    opencv_image2sam_binarymask(mask_sam, mask_opencv);
    return true;//TODO
    */
}

//multiple points, image sam format
bool SAM2Segmentor::get_best_mask_at_points(std::vector<MaskPoint>& points, sam_image_u8& image_sam, sam_image_u8& mask_sam) {
    cv::Mat image_opencv;
    sam_image_color2opencv(image_sam, image_opencv);
    cv::Mat mask_opencv = get_best_mask_at_points(points, image_opencv);
    opencv_image2sam_binarymask(mask_sam, mask_opencv);
    return true;//TODO
}


void SAM2Segmentor::close() {
    //TODO
    //sam_deinit(*a_sam_state);
}
