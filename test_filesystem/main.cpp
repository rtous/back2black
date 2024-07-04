#include <stdio.h>
#include "sam.h"

#include <string>
#include <iostream>
#include "filesystem_header.h"

#include <opencv2/opencv.hpp> 
#include <opencv2/core/utils/filesystem.hpp>

#include "common1.h"
#include "data_structures.h"

#if defined(_MSC_VER)
#pragma warning(disable: 4244 4267) // possible loss of data
#endif



// Main code
int main(int argc, char ** argv) 
{

    std::string input_path = "data/example1/images";
    std::string output_path = "output/example1/masks";

    if (!fs::exists(output_path)) {
        printf("Output directory does not exist, creating: %s", output_path.c_str());
        cv::utils::fs::createDirectories(output_path);
    }
	
	std::vector<fs::directory_entry> files_in_directory;
    std::copy(fs::directory_iterator(input_path), fs::directory_iterator(), std::back_inserter(files_in_directory));
    std::sort(files_in_directory.begin(), files_in_directory.end());
	

    /*for (const auto & entry : files_in_directory) {
        std::cout << entry.path() << std::endl;
		//std::string filename = entry.path().filename();
		std::string filename = entry.path();

	}*/
        /*std::string filename = entry.path().filename();
        std::string filename_noext = filename.substr(0, filename.find_last_of(".")); 

        if (entry.path().extension() == ".jpg" || entry.path().extension() == ".png") {
            sam_image_u8 img0;*/



    return 0;
}
