#include <stdio.h>
#include "sam.h"

#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::__fs::filesystem; //Maybe a problem of the Mac

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <opencv2/opencv.hpp> 
#include <opencv2/core/utils/filesystem.hpp>

#include "common1.h"
#include "data_structures.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4244 4267) // possible loss of data
#endif

static bool load_image_from_file(const std::string & fname, sam_image_u8 & img) {
    int nx, ny, nc;
    auto data = stbi_load(fname.c_str(), &nx, &ny, &nc, 3);
    if (!data) {
        fprintf(stderr, "%s: failed to load '%s'\n", __func__, fname.c_str());
        return false;
    }
    if (nc != 3) {
        fprintf(stderr, "%s: '%s' has %d channels (expected 3)\n", __func__, fname.c_str(), nc);
        return false;
    }

    img.nx = nx;
    img.ny = ny;
    img.data.resize(nx * ny * 3);
    memcpy(img.data.data(), data, nx * ny * 3);

    stbi_image_free(data);

    return true;
}

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


/*
    propagate_masks
*/
void propagate_masks(Frame baseFrame, std::vector<Frame> toFrames) 
{

    for (Object & anObject : baseFrame.objects) {
        //for (Contour & aContour : anObject.contours) {
        //we know the contour center, r
        printf("\tprocessing object %d...\n", anObject.objectId);
        for (Frame & aFrame : toFrames) {
            //obtain mask at reference point
            //update reference point
        }
    }

}

// Main code
int main(int argc, char ** argv) 
{

    printf("CLI tool v 0.1");
    
    sam_params params;
    if (!params_parse(argc, argv, params)) {
        return 1;
    }

    if (params.seed < 0) {
        params.seed = time(NULL);
    }
    fprintf(stderr, "%s: seed = %d\n", __func__, params.seed);

    std::string input_path = "data/example1/images";
    std::string output_path = "output/example1/masks";

    if (!fs::exists(output_path)) {
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

    /**********/
    //Traverse the directory
    //std::string path = params.fname_inp;
    //std::string path = "/Users/rtous/dev/back2black/data/example1/images";


    //To traverse the directory alphabetically:
    //Necessary to process the frames in order
    std::vector<fs::directory_entry> files_in_directory;
    std::copy(fs::directory_iterator(input_path), fs::directory_iterator(), std::back_inserter(files_in_directory));
    std::sort(files_in_directory.begin(), files_in_directory.end());

    sam_point pt { 500, 350};
    int contour_area = -1;
    printf("INITIAL POINT: pt.x=%f, pt.y=%f\n", pt.x, pt.y);

    //for (const auto & entry : fs::directory_iterator(input_path)) { //no alphabetical
    for (const auto & entry : files_in_directory) {
        std::cout << entry.path() << std::endl;

        std::string filename = entry.path().filename();
        std::string filename_noext = filename.substr(0, filename.find_last_of(".")); 

        if (entry.path().extension() == ".jpg" || entry.path().extension() == ".png") {
            // load the image
            sam_image_u8 img0;
            if (!load_image_from_file(entry.path(), img0)) {
                fprintf(stderr, "%s: failed to load image from '%s'\n", __func__, entry.path().c_str());
                return 1;
            }
            fprintf(stderr, "%s: loaded image '%s' (%d x %d)\n", __func__, entry.path().c_str(), img0.nx, img0.ny);
        
            //Compute image
            if (!sam_compute_embd_img(img0, params.n_threads, *state)) {
                fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
                return 1;
            }
            printf("t_compute_img_ms = %d ms\n", state->t_compute_img_ms);

            //compute masks for a given point
            std::vector<sam_image_u8> masks;
            
            masks = sam_compute_masks(img0, params.n_threads, pt, *state);

            printf("found %d masks\n", masks.size());

            //mask to opencv
            /*int mask_num = 0;
            cv::Mat output;
            for (auto& mask : masks) {
                printf("found mask\n");
                if (mask_num>0) {
                    printf("WARNING: MORE THAN ONE MASK FOUND\n");
                    //break;
                }
                //Opencv is y,x (fil, col)
                if (mask_num)
                    output = cv::Mat::zeros(mask.ny, mask.nx, CV_8UC1 );
                for (int i=0; i < output.rows; ++i){
                    for (int j=0; j < output.cols; ++j){
                        //output.at<uchar>(j, i) = mask.data[j*mask.nx+i];
                        output.at<uchar>(i, j) = mask.data[i*mask.nx+j];
                    }
                }
                mask_num++;
            }*/


            //mask to opencv
            int mask_num = 0;
            //sam_image_u8 mask = masks[masks.size()-1];
            for (auto& mask : masks) {
                printf("found mask\n");
                if (mask_num>0) {
                    printf("WARNING: MORE THAN ONE MASK FOUND\n");
                    //break;
                }
                //Opencv is y,x (fil, col)
                cv::Mat output = cv::Mat::zeros(mask.ny, mask.nx, CV_8UC1 );
                for (int i=0; i < output.rows; ++i){
                    for (int j=0; j < output.cols; ++j){
                        //output.at<uchar>(j, i) = mask.data[j*mask.nx+i];
                        output.at<uchar>(i, j) = mask.data[i*mask.nx+j];
                    }
                }

                std::vector<std::vector<cv::Point> > contours;
                std::vector<cv::Vec4i> hierarchy;
                cv:findContours(output, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE );

                //TODO: Multiple contours

                int new_contour_area = cv::contourArea(contours[0]);
                printf("new_contour_area = %d \n", new_contour_area);

                if (new_contour_area > 1000) {

                    // compute the center of the contour https://pyimagesearch.com/2016/02/01/opencv-center-of-contour/
                    cv::Moments M = cv::moments(contours[0]);
                    cv::Point center(M.m10/M.m00, M.m01/M.m00);  
                    circle(output, center, 5, cv::Scalar(128,0,0), -1);   

                    //contour area
                    
                    if (contour_area == -1 || (new_contour_area < contour_area*1.2 && new_contour_area > contour_area*0.8)) {
                        contour_area = new_contour_area; 
                        pt.x = center.x;
                        pt.y = center.y;
                        printf("UPDATED POINT: pt.x=%f, pt.y=%f\n", pt.x, pt.y);
                    } else {
                        printf("POINT NOT UPDATED BECAUSE CONTOUR AREA DIFFERS (BEFORE: %d, NOW: %d) \n", contour_area, new_contour_area);
                    }
                    
                    //for (int i = 0; i < mask.nx*mask.ny; i=i+1) {
                    //    output.at<uchar>(i, j) = 1;
                    //    output[0] = mask.data[i];
                    //    output[1] = mask.data[i+1];
                    //    output[2] = mask.data[i+2];
                    //}

                    cv::imwrite(output_path+"/"+filename_noext+"_"+std::to_string(mask_num)+".png", output);
                    mask_num++; 
                }
            }
        }   
        
    }

    sam_deinit(*state);

    return 0;
}
