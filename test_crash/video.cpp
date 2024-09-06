/*
 * Utils related to the video 
 */

#include <stdio.h>
#include "video.h"
#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/video/video.hpp>
#include "common1.h"




void read_video(const std::string &videoFilePath, Video &theVideo){//(std::vector<cv::Mat img> & frames) {
    printf("reading video...\n");
    //cv::Mat img = cv::imread(input_path+"/001.png");
    try{
		std::cerr << "Trying to open the video...\n";
        //open the video file
        cv::VideoCapture cap(videoFilePath); // open the video file
        if(!cap.isOpened())  // check if we succeeded
            CV_Error(cv::Error::StsError, "Can not open Video file");
        
        //cap.get(CV_CAP_PROP_FRAME_COUNT) contains the number of frames in the video;
		std::cerr << "Traversing frames...\n";
        for(int frameNum = 0; frameNum < cap.get(cv::CAP_PROP_FRAME_COUNT);frameNum++)
        {
            cv::Mat frame;
            cap >> frame; // get the next frame from video
            Frame aFrame;
            //aFrame.filePath = filepath;
            aFrame.img = frame;
            opencv_image2sam(aFrame.img_sam_format, frame);
            aFrame.order = frameNum;
            theVideo.frames.push_back(aFrame);
            printf("Added frame num %d\n", frameNum);  
        }
		std::cerr << "Traversing frames DONE\n";
        theVideo.loaded = true;
    }
    catch(cv::Exception& e ){
        std::cerr << e.msg << std::endl;
        exit(1);
    }
}