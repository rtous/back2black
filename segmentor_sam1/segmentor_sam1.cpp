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
    sam_image_u8 sam_image;
    opencv_image2sam(sam_image, opencv_image);
    if (!sam_compute_embd_img(sam_image, params.n_threads, *a_sam_state)) {
        fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
        return false;
    }
    return true;
}
