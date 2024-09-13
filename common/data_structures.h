#ifndef __DATASTRUCTURES_H__
#define __DATASTRUCTURES_H__

#include <opencv2/opencv.hpp> 
#include "sam.h"
//#include <SDL_opengl.h>

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
    
    //std::vector<GLuint> maskTextures;//vector of GLtextures of the all the masks of the object
    std::vector<sam_image_u8> masks; //vector of masks
    
    int objectId;
    //int color;
    float color[4]; //R,G,B,A
    int mask_computed_at_x;  
    int mask_computed_at_y;
    int mask_center_x;  
    int mask_center_y; 
    int mask_contour_size;
    std::vector<std::vector<cv::Point>> contours;

    Object() {
      printf("Object created!\n");
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
};

class Video {      
  public:       
    /*Video(string x) { 
      std::cout << "Z" << std::endl; 
    } */     
    std::vector<Frame> frames; 
    bool loaded = false;
};

#endif
