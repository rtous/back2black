#ifndef __STATE_H__
#define __STATE_H__

#include <string>
#include "sam.h"
#include "colors.h"
#include <SDL_opengl.h>
#include "data_structures.h"
#include "segmentor.h"
#include "segmentor_sam1.h"
#include "segmentor_sam2.h"

/*class MaskPoint {      
  public:             
    int x;
    int y;
    bool positive;
};*/

class MyState {      
  public: 

    //general
    bool DEBUG_MODE=false;

    //main
    bool done=false;

    //main window
    bool show_editor=true; 

    //menus
    bool about_version_popup=false;

    //size of the image frame (to resize the loaded images)
    int img_frame_w;
    int img_frame_h;

    //file dialog
    bool show_file_dialog = false;
    int file_dialog_mode = -1; //FILE_DIALOG_LOAD_SINGLE_FILE 0, FILE_DIALOG_LOAD_VIDEO 1, FILE_DIALOG_SAVE_VIDEO 2
    bool file_dialog_file_selected = false;  //set in file_dialog.cpp to signal the action //NOTE: In Windows it's not false by default
    std::string filePathName;
    std::string filePath;  
    bool img_loaded = false;

    //propagate dialog
    bool propagate_dialog = false; //to show the dialog
    bool propagate = false; //to perform the propagate action
    bool propagating = false; //during propagation
    bool propagated = false; //finished propagation
    bool propagate_cancel = false; //finished propagation
    float progress = 0.0f; //progress of propagate
    int start_frame = -1;
    int end_frame = -1;
    

    //SAM
    Segmentor& segmentor;
    sam_params params; 
    std::shared_ptr<sam_state> a_sam_state;

    //masks
    t_colors colors_palette;
    bool clicked; //the user clicked somewhere
    int clickedX; //raw imgui general windows coordinate
    int clickedY; //raw imgui general windows coordinate

    int mask_mode = 0; //0=click2mask, 1=positive, 2=negative
    std::vector<MaskPoint> sam2_points; //scaled to the original image
    bool mask_button_clicked = false; 
    
    //frames
    int selected_frame = 0;
    int selected_mask = -1; //set 0 if you want first mask selected by default
    int frame_precomputed = -1;
    Video aVideo;
    
    //image and texture in left panel:
    cv::Mat img_opencv;//NOT CURRENTLY USED, to be used with sam2?
    sam_image_u8 img_sam; //Used for processing and for rendering. 
    sam_image_u8 img_sam_format_downscaled; //Used for processing and for rendering. Currently downscaled to window size
    GLuint tex; 

    //finishing details
    float face_color[4]; //R,G,B,A (WARNING: values between 0 and 1)
    float eyes_color[4]; //R,G,B,A (WARNING: values between 0 and 1)
    //face(myState.aVideo.frames[myState.selected_frame].img, blankImageWithAlpha, cv::Scalar(118, 113, 168, 255), cv::Scalar(61, 71, 118, 255));
            
    int rimlight_size = 5;
    int pixelation_level = 5;

    bool change_color_all_frames = true;

    void reset() {  // Method/function defined inside the class
      aVideo = Video();
    }

    //as segmentor holds a reference it needs to be intitialized this way
    //"constructor initializer list"
    MyState() : segmentor(get_sam2_segmentor())  {
    //MyState() : segmentor(get_sam1_segmentor())  {
      //Default color
      face_color[0] = 168/(float)256;
      face_color[1] = 113/(float)256;
      face_color[2] = 118/(float)256;
      face_color[3] = 1.0f;//no transparency
      eyes_color[0] = 118/(float)256;
      eyes_color[1] = 71/(float)256;
      eyes_color[2] = 61/(float)256;
      eyes_color[3] = 1.0f;//no transparency
    }
};

#endif