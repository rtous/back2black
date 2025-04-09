#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/video/video.hpp>

using namespace cv;
int main()
{
	std::cerr << "Urk!\n";
	printf("trying to open and image with opencv...\n");
    //std::string image_path = samples::findFile("img.jpg");
    //std::string image_path = "/Users/rtous/dev/back2black/img.jpg";
	std::string image_path = "img.jpg";
    Mat img = imread(image_path, IMREAD_COLOR);
    if(img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    imshow("Display window", img);
    int k = waitKey(0); // Wait for a keystroke in the window
    if(k == 's')
    {
        imwrite("../out.jpg", img);
    }
	
	printf("trying to open a video with opencv...\n");

	std::string videoFilePath = "footage.mp4";
	
    try{
        //open the video file
        cv::VideoCapture cap(videoFilePath); // open the video file
        if(!cap.isOpened())  // check if we succeeded
            CV_Error(cv::Error::StsError, "Can not open Video file");
        
        //cap.get(CV_CAP_PROP_FRAME_COUNT) contains the number of frames in the video;
        for(int frameNum = 0; frameNum < cap.get(cv::CAP_PROP_FRAME_COUNT); frameNum++)
        {
            cv::Mat frame;
            cap >> frame; // get the next frame from video
            printf("Added frame num %d\n", frameNum);  
        }
    }
    catch(cv::Exception& e ){
        std::cerr << e.msg << std::endl;
        exit(1);
    }
	
    return 0;
}