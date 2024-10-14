#ifndef __STATE_H__
#define __STATE_H__

#include <string>
#include "sam.h"
#include "colors.h"
#include <SDL_opengl.h>
#include "data_structures.h"

class MyState {      
  public:  

    //position of the image frame within the application window
    //int img_frame_pos_x;
    //int img_frame_pos_y;

    //panels, etc.     
    bool show_file_dialog_video = false;      
    bool openFile;  
    bool openFileVideo;   
    std::string filePathName;
    std::string filePath;  
    bool img_loaded = false;

    //SAM
    sam_params params; 
    std::shared_ptr<sam_state> a_sam_state;

    //masks
    t_colors colors_palette;
    bool clicked;
    int clickedX;
    int clickedY;
    
    
    int selected_frame = 0;
    int selected_object = 0;
    int frame_precomputed = -1;
    Video aVideo;
    bool propagate = false;

    //single file case:
    sam_image_u8 img; 
    GLuint tex; 
};

#endif