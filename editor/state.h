#ifndef __STATE_H__
#define __STATE_H__

#include <string>
#include "sam.h"
#include <SDL_opengl.h>

class MyState {      
  public:             
    bool openFile;    
    std::string filePathName;
    std::string filePath;   
    sam_params params; 
    std::shared_ptr<sam_state> a_sam_state;
    std::vector<GLuint> maskTextures;
    std::vector<sam_image_u8> masks;
    bool clicked;
    int clickedX;
    int clickedY;
    sam_image_u8 img;
    bool img_loaded = false;
};

#endif