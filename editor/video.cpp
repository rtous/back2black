/*
 * Utils related to the video 
 */

#include <stdio.h>
#include "video.h"
#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/video/video.hpp>
#include "common1.h"
#include "imgui_impl_sdl2.h"
#include <SDL.h>
#include "gui_utils.h"
#include "simplify.h"

void read_video(const std::string &videoFilePath, Video &theVideo, int w, int h){//(std::vector<cv::Mat img> & frames) {
    printf("reading video...\n");
    //cv::Mat img = cv::imread(input_path+"/001.png");
    try{
        //open the video file
        cv::VideoCapture cap(videoFilePath); // open the video file
        if(!cap.isOpened())  // check if we succeeded
            CV_Error(cv::Error::StsError, "Can not open Video file");
        
        //cap.get(CV_CAP_PROP_FRAME_COUNT) contains the number of frames in the video;
        for(int frameNum = 0; frameNum < cap.get(cv::CAP_PROP_FRAME_COUNT);frameNum++)
        {
            cv::Mat frame;
            cap >> frame; // get the next frame from video
            Frame aFrame;
            //aFrame.filePath = filepath;
            aFrame.img = frame;
            opencv_image2sam(aFrame.img_sam_format, frame);
            printf("SAM (%d,%d) FROM OPENCV (%d,%d): \n", aFrame.img_sam_format.nx, aFrame.img_sam_format.ny, frame.cols, frame.rows);

            aFrame.order = frameNum;
            //viewport->Window
            //SDL_Window* sdl_window = SDL_GetWindowFromID(gpu_screen->context->windowID); 
            downscale_img_to_size(aFrame.img_sam_format, aFrame.img_sam_format_downscaled, w, h);
            printf("DOWNSCALED SAM TO (%d,%d) video.cpp: \n", aFrame.img_sam_format.nx, aFrame.img_sam_format.ny);

            aFrame.tex = createGLTexture(aFrame.img_sam_format, GL_RGB);

            theVideo.frames.push_back(aFrame);
            printf("Added frame num %d\n", frameNum);  
        }
        theVideo.loaded = true;
    }
    catch(cv::Exception& e ){
        std::cerr << e.msg << std::endl;
        exit(1);
    }
}
//accepts only mp4?
void save_video(MyState &myState, const std::string &videoFilePath, Video &theVideo){//(std::vector<cv::Mat img> & frames) {
    printf("saving video to %s...\n", videoFilePath.c_str());
    int fps = 25;
    cv::Size frame_size = theVideo.frames[0].img.size();    
    //https://docs.opencv.org/3.4/d7/d9e/tutorial_video_write.html
    //cv::VideoWriter vidwriter(videoFilePath, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frame_size, true);
    
    /*string::size_type pAt = source.find_last_of('.');                  // Find extension point
    const string NAME = source.substr(0, pAt) + argv[2][0] + ".avi";   // Form the new name with container
    int ex = static_cast<int>(inputVideo.get(CAP_PROP_FOURCC));     // Get Codec Type- Int form
    // Transform from int to char via Bitwise operators
    char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};
    */

    //cv::VideoWriter vidwriter(videoFilePath.c_str(), ex=-1, fps, frame_size, true);
    cv::VideoWriter vidwriter;
    //int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G'); //-1
    //int codec = cv::VideoWriter::fourcc('M', 'P', '4', 'V');    
    int codec = cv::VideoWriter::fourcc('H', '2', '6', '4');
    //int codec = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
    vidwriter.open(videoFilePath, codec, fps, frame_size, true);
    if (!vidwriter.isOpened())
    {
        printf("Could not open the output video for write: %s\n", videoFilePath.c_str());
    } else {
        //cv::Mat img = cv::imread(input_path+"/001.png");
        try {
            int numFrames = theVideo.frames.size();
            int f = 0;
            //iterate through all frames 
            for (Frame & aFrame : theVideo.frames) {
                printf("PROCESSING FRAME %d \n", f);
                if (aFrame.masks.size()>0) {
                    /*
                    //OLD WAY
                    //iterate through all the masks of the frame
                    cv::Mat output_image_opencv = cv::Mat::zeros(aFrame.img.size(), CV_8UC4);;
                    for (Mask & aMask : aFrame.masks) {
                        printf("\tPROCESSING MASK %d \n", aMask.maskId);
                        //from simplify.cpp
                        fillContoursWithColorAndAlpha(aMask.simplifiedContours, output_image_opencv, false, aMask.color[0]*255, aMask.color[1]*255, aMask.color[2]*255);
                        printf("\tPROCESSING MASK DONE.\n");
                    }
                    cv::Size a_frame_size = aFrame.img.size(); 
                    printf("writing frame with size %d,%d.\n", a_frame_size.width, a_frame_size.height);
                    
                    cv::Mat bgr;
                    cv::cvtColor(output_image_opencv, bgr, cv::COLOR_RGB2BGR);
                    vidwriter.write(bgr);
                    */

                    //New way, directly aFrame.img_simplified
                    //cv::Mat outImg; 
                    //Facial traits
                    cv::Mat img_simplified_withFace = aFrame.img_simplified.clone();
                    if (aFrame.faces_computed && aFrame.faces_check) {
                        //we are working in BGRA so we need to switch from RGBA
                        cv::Scalar faceColor = cv::Scalar(myState.face_color[2]*255, myState.face_color[1]*255, myState.face_color[0]*255, myState.face_color[3]*255);
                        cv::Scalar pupilsColor = cv::Scalar(myState.eyes_color[2]*255, myState.eyes_color[1]*255, myState.eyes_color[0]*255, myState.eyes_color[3]*255);
                        //overlay(img_simplified_withFace, img_simplified_withFace, aFrame.faces);
                        img_simplified_withFace.setTo(faceColor, aFrame.faces);
                        //overlay(img_simplified_withFace, img_simplified_withFace, aFrame.eyes);
                        img_simplified_withFace.setTo(pupilsColor, aFrame.eyes);
                    }
                    //img_simplified_withFace = aFrame.faces.clone();
                    //TODO: Instead of rescaling work with native size
                    cv::Mat outImg;                    
                    cv::resize(img_simplified_withFace, outImg, aFrame.img.size(), 0, 0, cv::INTER_LINEAR);
                    vidwriter.write(outImg);        
                }
                printf("FRAME DONE.\n");
                f++;
            }
            vidwriter.release();
        } catch(cv::Exception& e ){
            std::cerr << e.msg << std::endl;
            exit(1);
        }
    }
}

/*
void save_video_frames(const std::string &dirPath, Video &theVideo){//(std::vector<cv::Mat img> & frames) {
    if (!cv::utils::fs::exists(dirPath)) {
        printf("Output directory does not exist, creating: %s", dirPath.c_str());
        cv::utils::fs::createDirectories(dirPath);
    }
    printf("saving video to %s...\n", dirPath.c_str());
    int fps = 25;
    cv::Size frame_size = theVideo.frames[0].img.size();    
    
    int numFrames = theVideo.frames.size();
    int f = 0;
    //iterate through all frames 
    for (Frame & aFrame : theVideo.frames) {
        printf("PROCESSING FRAME %d \n", f);
        if (aFrame.masks.size()>0) {
            //iterate through all the masks of the frame
            cv::Mat output_image_opencv = cv::Mat::zeros(aFrame.img.size(), CV_8UC4);;
            for (Mask & aMask : aFrame.masks) {
                printf("\tPROCESSING MASK %d \n", aMask.maskId);
                //from simplify.cpp
                fillContoursWithColorAndAlpha(aMask.simplifiedContours, output_image_opencv, false, aMask.color[0]*255, aMask.color[1]*255, aMask.color[2]*255);
                printf("\tPROCESSING MASK DONE.\n");
            }
            cv::Size a_frame_size = aFrame.img.size(); 
            printf("writing frame with size %d,%d.\n", a_frame_size.width, a_frame_size.height);
            
            cv::Mat bgra;
            cv::cvtColor(output_image_opencv, bgra, cv::COLOR_RGBA2BGRA);
            std::string frame_path = dirPath+"/frame"+ std::to_string(f)+".png";
            printf("saving frame to %s...\n", frame_path.c_str());
            cv::imwrite(frame_path, bgra);

            
        }
        printf("FRAME DONE.\n");
        f++;
    }
        
}
*/
void save_video_frames(MyState &myState, const std::string &dirPath, Video &theVideo){//(std::vector<cv::Mat img> & frames) {
    if (!cv::utils::fs::exists(dirPath)) {
        printf("Output directory does not exist, creating: %s", dirPath.c_str());
        cv::utils::fs::createDirectories(dirPath);
    }
    printf("saving video to %s...\n", dirPath.c_str());
    int fps = 25;
    cv::Size frame_size = theVideo.frames[0].img.size();    
    
    int numFrames = theVideo.frames.size();
    int f = 0;
    //iterate through all frames 
    for (Frame & aFrame : theVideo.frames) {
        printf("PROCESSING FRAME %d \n", f);
        if (aFrame.masks.size()>0) {
            
            cv::Mat img_simplified_withFace = aFrame.img_simplified.clone();
            if (aFrame.faces_computed && aFrame.faces_check) {
                //we are working in BGRA so we need to switch from RGBA
                cv::Scalar faceColor = cv::Scalar(myState.face_color[2]*255, myState.face_color[1]*255, myState.face_color[0]*255, myState.face_color[3]*255);
                cv::Scalar pupilsColor = cv::Scalar(myState.eyes_color[2]*255, myState.eyes_color[1]*255, myState.eyes_color[0]*255, myState.eyes_color[3]*255);
                //overlay(img_simplified_withFace, img_simplified_withFace, aFrame.faces);
                img_simplified_withFace.setTo(faceColor, aFrame.faces);
                //overlay(img_simplified_withFace, img_simplified_withFace, aFrame.eyes);
                img_simplified_withFace.setTo(pupilsColor, aFrame.eyes);
            }
            //img_simplified_withFace = aFrame.faces.clone();
            //TODO: Instead of rescaling work with native size
            cv::Mat outImg;                    
            cv::resize(img_simplified_withFace, outImg, aFrame.img.size(), 0, 0, cv::INTER_LINEAR);


            //cv::Mat bgra;
            //cv::cvtColor(img_simplified_withFace, bgra, cv::COLOR_RGBA2BGRA);
            std::string frame_path = dirPath+"/frame"+ std::to_string(f)+".png";
            printf("saving frame to %s...\n", frame_path.c_str());
            //cv::imwrite(frame_path, bgra);  
            cv::imwrite(frame_path, img_simplified_withFace);  

        }
        printf("FRAME DONE.\n");
        f++;
    }
        
}

//save as a single frame
void save_frame(const std::string &imageFilePath, Video &theVideo, int selected_frame){//(std::vector<cv::Mat img> & frames) {
    printf("saving frame to %s...\n", imageFilePath.c_str());
    int fps = 25;
    cv::Size frame_size = theVideo.frames[selected_frame].img.size();    
    
    if (theVideo.frames[selected_frame].masks.size()>0) {
        //iterate through all the masks of the frame
        cv::Mat output_image_opencv = cv::Mat::zeros(theVideo.frames[selected_frame].img.size(), CV_8UC4);;
        for (Mask & aMask : theVideo.frames[selected_frame].masks) {
            printf("\tPROCESSING MASK %d \n", aMask.maskId);
            //from simplify.cpp
            fillContoursWithColorAndAlpha(aMask.simplifiedContours, output_image_opencv, false, aMask.color[0]*255, aMask.color[1]*255, aMask.color[2]*255);
            printf("\tPROCESSING MASK DONE.\n");
        }
        cv::Size a_frame_size = theVideo.frames[0].img.size(); 
        printf("writing frame with size %d,%d.\n", a_frame_size.width, a_frame_size.height);
        
        //TODO: Did this because aFrame.img is RGB instead of BGR
        //cv::Mat bgr;
        //cv::cvtColor(aFrame.img, bgr, cv::COLOR_RGB2BGR);
        
        cv::Mat bgra;
        cv::cvtColor(output_image_opencv, bgra, cv::COLOR_RGBA2BGRA);
        cv::imwrite(imageFilePath, bgra);
        
    }
    printf("FRAME DONE.\n");         
    
}