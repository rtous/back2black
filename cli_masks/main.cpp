#include <stdio.h>
#include "sam.h"

#include <string>
#include <iostream>
//#include <filesystem>

#include "filesystem_header.h"

#include <opencv2/opencv.hpp> 
#include <opencv2/core/utils/filesystem.hpp>

#include "common1.h"
#include "data_structures.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4244 4267) // possible loss of data
#endif

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
    //DEBUG TEST
    /*Mask aMask;
    example_func(aMask);
    printf("aMask.maskId = %d \n", aMask.maskId);
    */
    ///////////

    printf("CLI tool v 0.1");
    
    sam_params params;
    if (!params_parse(argc, argv, params)) {
        return 1;
    }

    if (params.seed < 0) {
        params.seed = time(NULL);
    }
    fprintf(stderr, "%s: seed = %d\n", __func__, params.seed);

    std::string input_path = "data/example2/images";
    std::string output_path = "output/example2/masks";

    if (!cv::utils::fs::exists(output_path)) {
        printf("Output directory does not exist, creating: %s", output_path.c_str());
        cv::utils::fs::createDirectories(output_path);
    }


    /**********/
    //Load SAM MODEL
    std::shared_ptr<sam_state> state = sam_load_model(params);
    if (!state) {
        fprintf(stderr, "%s: failed to load model\n", __func__);
        return 1;
    }
    printf("t_load_ms = %d ms\n", state->t_load_ms);

    
	
    //sam_point pt { 500, 347}; 539, 309
    //sam_point pt { 539, 309};
    int contour_area = -1;
    //printf("INITIAL POINT: pt.x=%f, pt.y=%f\n", pt.x, pt.y);

    std::vector<Frame> frames;
	
    //load frames from the directory
    load_frames_from_files(input_path, frames);

    //precompute first frame
    if (!sam_compute_embd_img(frames[0].img_sam_format, params.n_threads, *state)) {
        fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
        return false;
    }

    //Manually add two masks to the first frame
    Mask aMask;
    aMask.maskId = 0;
    aMask.mask_computed_at_x = 539;
    aMask.mask_computed_at_y = 309;
    compute_mask(aMask, frames[0].img_sam_format, *state, params.n_threads);
    frames[0].masks.push_back(aMask);
    printf("frames[0].masks[0].mask_center_x=%d\n", frames[0].masks[0].mask_center_x);

    Mask aMask2;
    aMask2.maskId = 1;
    aMask2.mask_computed_at_x = 500;
    aMask2.mask_computed_at_y = 347; 
    compute_mask(aMask2, frames[0].img_sam_format, *state, params.n_threads);
    frames[0].masks.push_back(aMask2);
    printf("frames[0].masks[1].mask_center_x=%d\n", frames[0].masks[1].mask_center_x);
            
    printf("frames.size()=%d\n",frames.size());


    //Propagate the masks (currently only to 5 frames)
    float progress;
    bool cancel = false;
    propagate_masks(frames, *state, params.n_threads, 0, 4, progress, cancel);


    printf("ANALYSIS DONE, WRITING IMAGE FILES!\n");
    /////////////
    // write masks to files
    ////////////
    int f = 0;
    for (Frame & aFrame : frames) {
        printf("PROCESSING FRAME %d \n", f);
        std::string filename = aFrame.filePath.substr(aFrame.filePath.find_last_of("/\\") + 1);
        std::string filename_noext = filename.substr(0, filename.find_last_of(".")); 
        int o = 0;
        for (Mask & aMask : aFrame.masks) {
            printf("PROCESSING MASK %d \n", o);
            std::string path = output_path+"/"+filename_noext+"_"+std::to_string(o)+".png";
            
            cv::imwrite(path, aMask.opencv_mask);
            printf("File written: %s\n", path.c_str());
            o++;
        }
        f++;
    }

    sam_deinit(*state);

    return 0;
}
