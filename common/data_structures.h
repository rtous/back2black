#ifndef __DATASTRUCTURES_H__
#define __DATASTRUCTURES_H__

#include <opencv2/opencv.hpp> 

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
    cv::Mat mask;
    int objectId;
    int color;
};

class Frame {      
  public:             
    int order; //from 0 to N-1 
    bool loaded = false;
    std::string filePath;   
    std::vector<Object> objects; 
};

class Video {      
  public:             
    std::vector<Frame> frames; 
};

#endif
