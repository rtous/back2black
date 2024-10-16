#ifndef __STATE_H__
#define __STATE_H__

#include <string>
#include "sam.h"
#include "colors.h"
#include <SDL_opengl.h>
#include "data_structures.h"

class MyState {      
  public:  

    //size of the image frame (to resize the loaded images)
    int img_frame_w;
    int img_frame_h;

    //file dialog
    int file_dialog_mode = -1; //FILE_DIALOG_LOAD_SINGLE_FILE 0, FILE_DIALOG_LOAD_VIDEO 1, FILE_DIALOG_SAVE_VIDEO 2
    bool file_dialog_file_selected;  //set in file_dialog.cpp to signal the action
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
    int selected_mask = 0;
    int frame_precomputed = -1;
    Video aVideo;
    bool propagate = false;


    //single file case:
    sam_image_u8 img; 
    GLuint tex; 
};

#endif