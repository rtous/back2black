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

    //pixels
    sam_image_u8 samMask; //mask in sam format
    bool mask_computed = false;
    GLuint maskTexture; //mask in OpenGL
    GLuint simplifiedMaskTexture; //simplified mask in OpenGL
    bool textures_computed = false;

    //contours
    //WHO's using this??
    std::vector<std::vector<cv::Point>> contours;//contours of the mask

    std::vector<std::vector<cv::Point>> simplifiedContours;//contours of the simplified mask

  
    
    int objectId;
    //int color;
    float color[4]; //R,G,B,A (WARNING: values between 0 and 1)
    int mask_computed_at_x = -1;  
    int mask_computed_at_y = -1;
    int mask_center_x = -1;  
    int mask_center_y = -1; 
    int mask_contour_size;

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
    sam_image_u8 img_sam_format; //the video frame in sam format
    GLuint tex; //OpenGL texture of the original video frame

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
