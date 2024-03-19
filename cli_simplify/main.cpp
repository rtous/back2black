#include <stdio.h>

#include <opencv2/opencv.hpp> 
//using namespace cv; 

/*#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>*/

#include "sam.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

static bool pixel_equal(const cv::Vec3b &a, const cv::Vec3b &b)
{  
    return a == b;   
}

/*
int unique_colors(cv::Mat img)
{
    // partition wants a vector, so we need a copy ;(
    cv::Vec3b *p = img.ptr<cv::Vec3b>();
    std::vector<cv::Vec3b> pix(p, p+img.total());

    // now cluster:
    std::vector<int> labels;
    int unique = cv::partition(pix, labels, pixel_equal);

    return unique;
}*/

//unique_colors
//std::vector unique_colors(cv::Mat img_gray, std::vector unique_colors) 
int unique_colors(cv::Mat img_gray, std::map<int,cv::Mat> & unique_colors, std::map<int,int> & pixels_per_colour) 
{
    printf("Finding unique colors...\n");

    //std::map<int,int> pixels_per_colour;

    cv::Mat img_smaller;
    //cv::resize(img_gray, img_smaller, cv::Size(), 0.25, 0.25); 
    //cv::Mat img_smaller = img_gray;

    uchar color;
    int total_colors = 0;
    for(int i=0; i<img_gray.rows; i++) {
        for(int j=0; j<img_gray.cols; j++) {
            
            color = img_gray.at<uchar>(i,j);
            if (color != 0) {
                if (unique_colors.find(color) == unique_colors.end()) {
                    cv::Mat mask = cv::Mat::zeros(img_gray.size(), img_gray.type());
                    mask.at<uchar>(i,j) = 255;
                    unique_colors.insert(std::pair<int, cv::Mat>(color, mask));
                    pixels_per_colour.insert(std::pair<int, int>(color, 0));
                    total_colors++;
                } else {
                    cv::Mat mask = unique_colors.at(color);
                    mask.at<uchar>(i,j) = 255;
                    pixels_per_colour[color]++;
                }
            }

        }
        //printf("\n");
    }
    return total_colors;
}



// Main code
int main(int argc, char ** argv) 
{
    printf("CLI SIMPLIFY tool v 0.1\n");

    /****** LOAD IMAGE **********/
    //im = cv2.imread(os.path.join(inputpath, filename))
    //cv::Mat img = cv::imread("img.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img = cv::imread("data/example1/masks/001.png");

    if (img.empty()) { 
        printf("Image not found.");
        return -1; 
    } else {
        printf("Image successfully read.\n");


    }

    /******* TO GRAYSCALE *********/
    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);




    /******** COLOR SEGMENTS *********/
    //int nc = unique_colors(img);
    std::map<int,cv::Mat> unique_colors_map;
    std::map<int,int> pixels_per_colour;
    int nc = unique_colors(img_gray, unique_colors_map, pixels_per_colour);
    

    printf("Total colors = %d.\n", nc);


    /******** CONTOURS *********/

    int i = 0;
    //for each color segment
    for (std::map<int,cv::Mat>::iterator it=unique_colors_map.begin(); it!=unique_colors_map.end(); ++it) {
        printf("Writing mask for color %d\n", it->first);
        if (pixels_per_colour[it->first] > 500) {
            cv::Mat mask = it->second;
            cv::imwrite("mask"+std::to_string(i)+".jpg", mask);

            std::vector<std::vector<cv::Point> > contours;
            std::vector<cv::Vec4i> hierarchy;
            cv:findContours(mask, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE );

            cv::RNG rng(12345);//random number generator
            //for each contour
            for( int j = 0; j< contours.size(); j++ )
            {
                if (cv::contourArea(contours[j]) > 10) {
                    cv::Mat drawing = cv::Mat::zeros( mask.size(), CV_8UC3 );
                    cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
                    cv::drawContours( drawing, contours, (int)j, color, 2, cv::LINE_8, hierarchy, 0 );
                    cv::imwrite("mask_contour"+std::to_string(i)+"_"+std::to_string(j)+".jpg", drawing);


                    /**** DILATE ****/

                    /**** SIMPLIFY ***/

                    std::vector<cv::Point> contoursOUT;
                    cv::approxPolyDP( cv::Mat(contours[j]), contoursOUT, 4, false );
                    contours[j] = contoursOUT;

                    drawing = cv::Mat::zeros( mask.size(), CV_8UC3 );
                    cv::drawContours(drawing, contours, (int)j, color, 2, cv::LINE_8, hierarchy, 0 );
                    cv::imwrite("mask_contour_simplified"+std::to_string(i)+"_"+std::to_string(j)+".jpg", drawing);

                }
            }
            
            //cv::imwrite("output.jpg", drawing);

            i++;
        }
    }
       



    /******* CONTOURS *********/
    /*
    cv::Mat canny_output;
    int thresh = 100;
    cv::Canny(img_gray, canny_output, thresh, thresh*2 );
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    //cv:findContours(canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
    cv:findContours(img_gray, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE );

    //Draw
    cv::RNG rng(12345);//random number generator
    cv::Mat drawing = cv::Mat::zeros( canny_output.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        cv::drawContours( drawing, contours, (int)i, color, 2, cv::LINE_8, hierarchy, 0 );
    }
    //cv::imshow( "Contours", drawing );

    cv::imwrite("output.jpg", drawing);

    */


    return 0;
}
