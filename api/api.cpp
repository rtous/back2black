#include "api.h"
#include "common1.h"
#include <opencv2/core/utils/filesystem.hpp>
#include "filesystem_header.h"

using namespace cv;

APIState::APIState() {
    params.n_threads = 16;
    params.model = "checkpoints/ggml-model-f16.bin";
    //state = sam_load_model(params);
}

void APIState::set_checkpoint_path(std::string checkpoint_path_) {
    checkpoint_path = checkpoint_path_;
}

void APIState::load_model() {
    params.model = checkpoint_path;
    if (!cv::utils::fs::exists(checkpoint_path)) {
        printf("Checkpoints path does not exist: %s.\n", checkpoint_path.c_str());
        exit(1);
    } else 
        state = sam_load_model(params);
}

void APIState::precompute_image(sam_image_u8 a_sam_image) {
    sam_image = a_sam_image;
    if (!sam_compute_embd_img(sam_image, params.n_threads, *state)) {
        fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
        exit(EXIT_FAILURE);
    }
}

void APIState::precompute_image_opencv(cv::Mat input_img) {
    sam_image_u8 sam_image;
    opencv_image2sam(sam_image, input_img);
    precompute_image(sam_image);
}

std::vector<sam_image_u8> APIState::compute_masks_samformat(sam_point pt) {
    std::vector<sam_image_u8> masks;
    masks = sam_compute_masks(sam_image, params.n_threads, pt, *state);
    return masks;
}

std::vector<sam_image_u8> APIState::compute_masks_samformat(int x, int y) {
    sam_point pt {(float)x, (float)y};
    std::vector<sam_image_u8> masks;
    masks = compute_masks_samformat(pt);
    return masks;
}

bool APIState::compute_mask_opencv(int x, int y, Mat & output_img) {
    std::vector<sam_image_u8> masks;
    masks = compute_masks_samformat(x, y);
    sam_image_u8 mask;
    if (masks.size()>0) {
        mask = masks[0]; //the returned masks are ordered by iou and stability_score
        sam_image_grayscale2opencv(mask, output_img);
        return true;
    } else return false;
}
