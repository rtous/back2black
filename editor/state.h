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
    bool clicked;
    int clickedX;
    int clickedY;
};

#endif