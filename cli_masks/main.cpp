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


/*
    propagate_masks
*/
/*void propagate_masks(Frame baseFrame, std::vector<Frame> toFrames) 
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

}*/

/*
//We assume the first frame has already the objects and the user coordinates
int propagate_masks(std::vector<Frame> & frames, sam_state & state, int n_threads) 
{
    int numFrames = frames.size();
    int f = 0;
    //iterate through all frames
    for (Frame & aFrame : frames) {
        printf("PROCESSING FRAME %d \n", f);
        //load the frame
        sam_image_u8 img0;
        if (!load_and_precompute_image_from_file(aFrame.filePath, img0, state, n_threads)) {
            fprintf(stderr, "%s: failed load_and_precompute_image_from_file from '%s'\n", __func__, aFrame.filePath.c_str());
            return 1;
        }

        //iterate through all the objects of the frame
        for (Object & anObject : aFrame.objects) {
            printf("\tPROCESSING OBJECT %d \n", anObject.objectId);
            compute_object(anObject, img0, state, n_threads);
            
            //if there are previous objects can check if the mask makes sense:
            //bool isMaskConsistent = true; //th first mask is consistent
            //if (f > 0)
            //    isMaskConsistent =  mask contour similar to previous mask contour

            
            //add the object to the next frame with the next coordinates
            printf("\tf=%d numFrames=%d.\n", f, numFrames);
            if (f < numFrames-1) {
                printf("\tADDING OBJECT %d TO FRAME %f.\n", anObject.objectId, f);
                Object newObject;
                newObject.objectId = anObject.objectId;
                newObject.mask_computed_at_x = anObject.mask_center_x;
                newObject.mask_computed_at_y = anObject.mask_center_y; 
                frames[f+1].objects.push_back(newObject);
            }
                  //if isMaskConsistent new coordinates are the mask coordinates
                  //otherwise use given coordinates
            //    frames[f+1].addObject(new coordinates); 
            printf("\tOBJECTS DONE.\n");
        }
        printf("FRAME DONE.\n");
        f++;
    }
}
*/

// Main code
int main(int argc, char ** argv) 
{
    //DEBUG TEST
    /*Object anObject;
    example_func(anObject);
    printf("anObject.objectId = %d \n", anObject.objectId);
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
	
	std::vector<std::string> filepaths_in_directory;
	cv::glob(input_path, filepaths_in_directory);


    ///////////////////////////
    //Create the list of frames with just the filepaths
    ///////////////////////////
    std::vector<Frame> frames;
    int f = 0;
    for (std::string filepath : filepaths_in_directory) {
        std::cout << filepath << std::endl;
        std::string filename = filepath.substr(filepath.find_last_of("/\\") + 1);
        std::string filename_noext = filename.substr(0, filename.find_last_of(".")); 
        std::string extension = filename.substr(filename.find_last_of(".")+1); 

        if (extension == "jpg" || extension == "png") {
            Frame aFrame;
            aFrame.filePath = filepath;
            //std::vector<Object> objects;

            if (f == 0) { //only add the objects in the first frame
                Object anObject;
                anObject.objectId = 0;
                //anObject.mask_computed_at_x = 500;
                //anObject.mask_computed_at_y = 350;
                anObject.mask_computed_at_x = 539;
                anObject.mask_computed_at_y = 309;
                aFrame.objects.push_back(anObject);

                Object anObject2;
                anObject2.objectId = 1;
                //anObject.mask_computed_at_x = 500;
                //anObject.mask_computed_at_y = 350;
                anObject2.mask_computed_at_x = 500;
                anObject2.mask_computed_at_y = 347;
                aFrame.objects.push_back(anObject2);



            }
            frames.push_back(aFrame);
            f++;
            
        }
    }
    ///////////////////////////

    propagate_masks(frames, *state, params.n_threads);

    printf("ANALYSIS DONE, WRITING IMAGE FILES!\n");
    /////////////
    // write masks to files
    ////////////
    f = 0;
    for (Frame & aFrame : frames) {
        printf("PROCESSING FRAME %d \n", f);
        std::string filename = aFrame.filePath.substr(aFrame.filePath.find_last_of("/\\") + 1);
        std::string filename_noext = filename.substr(0, filename.find_last_of(".")); 
        int o = 0;
        for (Object & anObject : aFrame.objects) {
            printf("PROCESSING OBJECT %d \n", o);
            std::string path = output_path+"/"+filename_noext+"_"+std::to_string(o)+".png";
            
            cv::imwrite(path, anObject.mask);
            printf("File written: %s\n", path.c_str());
            o++;
        }
        f++;
    }





    /*
    //Iterate all frames
    //for (const auto & entry : files_in_directory) {
	for (std::string filepath : filepaths_in_directory) {
        std::cout << filepath << std::endl;
		std::string filename = filepath.substr(filepath.find_last_of("/\\") + 1);
        std::string filename_noext = filename.substr(0, filename.find_last_of(".")); 
		std::string extension = filename.substr(filename.find_last_of(".")+1); 

        if (extension == "jpg" || extension == "png") {
            sam_image_u8 img0;

            //Precompute the frame
            if (!load_and_precompute_image_from_file(filepath, img0, *state, params.n_threads)) {
                fprintf(stderr, "%s: failed load_and_precompute_image_from_file from '%s'\n", __func__, filepath.c_str());
                return 1;
            }

            //Compute the frame: Obtain the best mask at the point
            cv::Mat output = get_best_opencv_mask_at_point(pt.x, pt.y, img0, *state, params.n_threads);

            //Obtain the first contour
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

                cv::imwrite(output_path+"/"+filename_noext+"_.png", output);
            }
            
        }   
        
    }
    */

    sam_deinit(*state);

    return 0;
}
