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
    printf("SAM 2 setup finished\n");
};

void SAM2Segmentor::test() 
{
    printf("called SAM2Segmentor::test()\n");
};

bool SAM2Segmentor::preprocessImage(cv::Mat & opencv_image) {
    printf("SAM2Segmentor::preprocessImage()\n");
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
