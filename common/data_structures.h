#ifndef __DATASTRUCTURES_H__
#define __DATASTRUCTURES_H__

#include <opencv2/opencv.hpp> 
#include "sam.h"
#include <SDL_opengl.h>

/*
	- A video is composed by frames
	- A frame is composed by objects
	- An object has ONE mask
	- An object has one-to-many contours
*/ 

class Contour {      
  public:             
    std::vector<cv::Point> contourPoints;
    cv::Point contourCenter;
};

class Object {      
  public:             
    cv::Mat mask;//old

    sam_image_u8 samMask;
    GLuint maskTexture;
    GLuint simplifiedMaskTexture;
  
    
    int objectId;
    //int color;
    float color[4]; //R,G,B,A (WARNING: values between 0 and 1)
    int mask_computed_at_x;  
    int mask_computed_at_y;
    int mask_center_x;  
    int mask_center_y; 
    int mask_contour_size;
    std::vector<std::vector<cv::Point>> contours;//contours of the object masks

    Object() {
      //Default color
      color[0] = (rand() % 256)/(float)256;
      color[1] = (rand() % 256)/(float)256;
      color[2] = (rand() % 256)/(float)256;
      color[3] = 0.0f;//no transparency
    }
};

class Frame {      
  public:             
    int order; //from 0 to N-1 
    bool loaded = false;
    std::string filePath;   
    std::vector<Object> objects; 
    cv::Mat img; //OpenCV format
    sam_image_u8 img_sam_format;
    GLuint tex; //OpenGL texture of the frame

    Frame() {     
      //starts with no objects
    }
};

class Video {      
  public:          
    std::vector<Frame> frames; 
    bool loaded = false;
};

#endif
