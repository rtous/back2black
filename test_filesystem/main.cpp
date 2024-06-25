#include <stdio.h>
#include "sam.h"

#include <string>
#include <iostream>
//#include <filesystem>
//#include <experimental/filesystem>
//namespace fs = std::__fs::filesystem; //Maybe a problem of the Mac
//namespace fs = std::experimental::filesystem;




#ifndef __has_include
  static_assert(false, "__has_include not supported");
#else
#  if __cplusplus >= 201703L && __has_include(<filesystem>)
#    include <filesystem>
     namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
     namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
     namespace fs = boost::filesystem;
#  endif
#endif



/*
Compila i linka en Mac però no en Windows
//#include <filesystem>
//namespace fs = std::__fs::filesystem; //Maybe a problem of the Mac
*/

/*
Compila però no linka en Windows
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
*/

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


    return 0;
}
