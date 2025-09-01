#include "stdio.h"
#include "segmentor_sam1.h"
#include "common1.h"

Segmentor& get_sam1_segmentor() 
{
    static SAM1Segmentor s1;
    return s1;
}

SAM1Segmentor::SAM1Segmentor() 
{
    printf("called SAM1Segmentor::SAM1Segmentor()\n");
    //./bin/editor -t 16 -i ../img.jpg -m ../checkpoints/ggml-model-f16.bin
    //params->seed = std::stoi(10);
    params.n_threads = 16;
    params.model = "checkpoints/ggml-model-f16.bin";
    //params->fname_inp = img_fname;
    //params.fname_out = argv[++i];
    a_sam_state = sam_load_model(params);
    if (!a_sam_state) {
        fprintf(stderr, "%s: failed to load model\n", __func__);
        //return 1;
    } else {
        a_sam_state = sam_load_model(params);
    }
    printf("SAM 1 setup finished\n");
};

void SAM1Segmentor::test() 
{
    printf("called SAM1Segmentor::test()\n");
};

bool SAM1Segmentor::preprocessImage(cv::Mat & opencv_image) {
    printf("SAM1Segmentor::preprocessImage()\n");
    sam_image_u8 image_sam;
    opencv_image2sam(image_sam, opencv_image);
    return preprocessImage(image_sam);
    /*if (!sam_compute_embd_img(image_sam, params.n_threads, *a_sam_state)) {
        fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
        return false;
    }
    return true;*/
}

bool SAM1Segmentor::preprocessImage(sam_image_u8 & image_sam) {
    if (!sam_compute_embd_img(image_sam, params.n_threads, *a_sam_state)) {
        fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
        return false;
    }
    return true;
}

bool SAM1Segmentor::preprocessImage_and_remember(cv::Mat & opencv_image) {
    //TODO
    return true;
}

bool SAM1Segmentor::preprocessImage_and_remember(sam_image_u8 & image_sam) {
    //TODO
    return true;
}

//es pot eliminar:
cv::Mat SAM1Segmentor::get_best_mask_at_point(int x, int y, cv::Mat& image_opencv) {
    //opencv image to sam
    sam_image_u8 image_sam;
    opencv_image2sam(image_sam, image_opencv);

    
    //compute mask at given point (pick best one)
    sam_image_u8 mask_sam;
    //bool found = get_best_sam_mask_at_point(x, y, image_sam, *a_sam_state, params.n_threads, mask);
    bool found = get_best_mask_at_point(x, y, image_sam, mask_sam); 

    //convert mask to opencv format
    cv::Mat mask_opencv;
    sam_image_grayscale2opencv(mask_sam, mask_opencv);

    return mask_opencv;
}

//es pot eliminar:
bool SAM1Segmentor::get_best_mask_at_point(int x, int y, sam_image_u8& image_sam, sam_image_u8& mask_sam) {
    //compute mask at given point (pick best one)
    bool found = get_best_sam_mask_at_point(x, y, image_sam, *a_sam_state, params.n_threads, mask_sam);
    return found;
}

bool SAM1Segmentor::get_best_mask_at_points(std::vector<MaskPoint>& points, sam_image_u8& image_sam, sam_image_u8& mask_sam) {
    //TODO
    return true;//TODO
}

bool SAM1Segmentor::get_best_mask_at_points_video(std::vector<MaskPoint>& points, sam_image_u8& image_sam, sam_image_u8& mask_sam, int frame_idx) {
    //TODO
    return true;//TODO
}

void SAM1Segmentor::reset_memory() {
    //TODO
}

void SAM1Segmentor::close() {
    sam_deinit(*a_sam_state);
}
