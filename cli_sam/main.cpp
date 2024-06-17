#include <stdio.h>
#include "sam.h"

#include <filesystem>
namespace fs = std::__fs::filesystem; //Maybe a problem of the Mac
#include <opencv2/opencv.hpp> 
#include <opencv2/core/utils/filesystem.hpp>

#if defined(_MSC_VER)
#pragma warning(disable: 4244 4267) // possible loss of data
#endif

#include "common1.h"

static void print_usage(int argc, char ** argv, const sam_params & params) {
    fprintf(stderr, "usage: %s [options]\n", argv[0]);
    fprintf(stderr, "\n");
    fprintf(stderr, "options:\n");
    fprintf(stderr, "  -h, --help            show this help message and exit\n");
    fprintf(stderr, "  -s SEED, --seed SEED  RNG seed (default: -1)\n");
    fprintf(stderr, "  -t N, --threads N     number of threads to use during computation (default: %d)\n", params.n_threads);
    fprintf(stderr, "  -m FNAME, --model FNAME\n");
    fprintf(stderr, "                        model path (default: %s)\n", params.model.c_str());
    fprintf(stderr, "  -i FNAME, --inp FNAME\n");
    fprintf(stderr, "                        input file (default: %s)\n", params.fname_inp.c_str());
    fprintf(stderr, "  -o FNAME, --out FNAME\n");
    fprintf(stderr, "                        output file (default: %s)\n", params.fname_out.c_str());
    fprintf(stderr, "\n");
}

static bool params_parse(int argc, char ** argv, sam_params & params) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-s" || arg == "--seed") {
            params.seed = std::stoi(argv[++i]);
        } else if (arg == "-t" || arg == "--threads") {
            params.n_threads = std::stoi(argv[++i]);
        } else if (arg == "-m" || arg == "--model") {
            params.model = argv[++i];
        } else if (arg == "-i" || arg == "--inp") {
            params.fname_inp = argv[++i];
        } else if (arg == "-o" || arg == "--out") {
            params.fname_out = argv[++i];
        } else if (arg == "-h" || arg == "--help") {
            print_usage(argc, argv, params);
            exit(0);
        } else {
            fprintf(stderr, "error: unknown argument: %s\n", arg.c_str());
            print_usage(argc, argv, params);
            exit(0);
        }
    }

    return true;
}

// Main code
int main(int argc, char ** argv) 
{
    printf("CLI SAM tool v 0.1");
    
    sam_params params;
    if (!params_parse(argc, argv, params)) {
        return 1;
    }

    if (params.seed < 0) {
        params.seed = time(NULL);
    }
    //fprintf(stderr, "%s: seed = %d\n", __func__, params.seed);


    std::string input_path = params.fname_inp;
    std::string output_path = "output/sam_test";

    if (!fs::exists(output_path)) {
        printf("Output directory does not exist, creating: %s", output_path.c_str());
        cv::utils::fs::createDirectories(output_path);
    }

    // load the image
    sam_image_u8 img0;
    if (!load_image_samformat_from_file(input_path, img0)) {
    //if (!load_image_from_file(input_path, img0)) {
        fprintf(stderr, "%s: failed to load image from '%s'\n", __func__, input_path.c_str());
        return 1;
    }
    fprintf(stderr, "%s: loaded image '%s' (%d x %d)\n", __func__, input_path.c_str(), img0.nx, img0.ny);

    std::shared_ptr<sam_state> state = sam_load_model(params);
    if (!state) {
        fprintf(stderr, "%s: failed to load model\n", __func__);
        return 1;
    }
    //printf("t_load_ms = %d ms\n", state->t_load_ms);


    if (!sam_compute_embd_img(img0, params.n_threads, *state)) {
        fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
        return 1;
    }
    //printf("t_compute_img_ms = %d ms\n", state->t_compute_img_ms);

    //int res = main_loop(std::move(img0), params, *state);


    /********************/

    //get mask at given point 
    cv::Point point; 
    point.x = 600;
    point.y = 450;
    cv::Mat output = get_best_opencv_mask_at_point(point.x, point.y, img0, *state, params.n_threads);
    circle(output, point, 5, cv::Scalar(128,0,0), -1);  
    cv::imwrite(output_path+"/mask.png", output);       

    //free sam resources
    sam_deinit(*state);

    return 0;
}
