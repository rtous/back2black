/*
 * Utils related to the editor (not general) but specific to sam and masks processing.
 */

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include "sam_utils.h"
#include "gui_utils.h"
#include "simplify.h"
#include "state.h"


void set_params(sam_params * params) {
    //./bin/editor -t 16 -i ../img.jpg -m ../checkpoints/ggml-model-f16.bin
    //params->seed = std::stoi(10);
    params->n_threads = 16;
    params->model = "checkpoints/ggml-model-f16.bin";
    //params->fname_inp = img_fname;
    //params.fname_out = argv[++i];
}

//for debugging
void print_mask(sam_image_u8 mask) {
    for (int i = 0; i < mask.nx; ++i) {
        for (int j = 0; j < mask.ny; ++j) {
            printf("%d,",mask.data[i*mask.ny+j]);
        }
        printf("\n");
    }
}
//for debugging
bool isEmptyMaskDEBUG(sam_image_u8 mask) {
    for (int i = 0; i < mask.nx*mask.ny; ++i)
        if (mask.data[i] != 0) 
            return false; 
    return true;
}

int masks_already_in_list(sam_image_u8 candidateMask, Frame & aFrame) {
    //printf("masks_already_in_list/isEmptyMaskDEBUG(mask)=%d\n", isEmptyMaskDEBUG(candidateMask));
    int mask_count = 0;
    printf("Checking if mask exists.\n");
    for (auto& mask : aFrame.masks) {
        if (mask.mask_computed) {//not checking empty masks
            sam_image_u8 samMask = mask.samMask;
            float pixels_1_any_of_both_masks = 0;
            float coincidences = 0;
            for (int i = 0; i < samMask.nx*samMask.ny; ++i) {
                if (candidateMask.data[i] != 0 || samMask.data[i] != 0) {
                    pixels_1_any_of_both_masks++;
                    if (candidateMask.data[i] == samMask.data[i])
                        coincidences++;
                }
            }
            printf("(mask %d) coincidences (%f/%f) = %f percent\n", mask_count, coincidences, pixels_1_any_of_both_masks, coincidences/pixels_1_any_of_both_masks);
            if (coincidences/pixels_1_any_of_both_masks > 0.75) {
                //printf("coincidences = %f percent", coincidences/pixels_1_any_of_both_masks);
                return mask_count;
            }
        }
        mask_count++;
    }
    printf("Not any equal.\n");
    return -1;
}


/*
	After the user clicked the image, compute a mask for that location.
	If the mask is already in storedMasks (myState.masks) it will be removed.

*/
/*void compute_masks(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y, std::vector<sam_image_u8> & storedMasks, std::vector<int> * mask_colors, int & last_color_id) {
    printf("compute_masks\n");
    std::vector<sam_image_u8> masks;
    //std::vector<GLuint> maskTextures;
    //float x = 100.f;
    //float y = 100.f;
    sam_point pt { static_cast<float>(x), static_cast<float>(y)};
    printf("pt = (%f, %f)\n", pt.x, pt.y);

    masks = sam_compute_masks(img, params.n_threads, pt, state);

    std::vector<int> masksToDelete;
    for (auto& mask : masks) {
        sam_image_u8 mask_rgb = { mask.nx, mask.ny, };
        mask_rgb.data.resize(4*mask.nx*mask.ny);
        for (int i = 0; i < mask.nx*mask.ny; ++i) {
            mask_rgb.data[4*i+0] = mask.data[i];
            mask_rgb.data[4*i+1] = mask.data[i];
            mask_rgb.data[4*i+2] = mask.data[i];
            mask_rgb.data[4*i+3] = mask.data[i];
        }
        int pos = masks_already_in_list(mask, storedMasks);
		
        if (pos == -1) { //the mask is new, not in storedMasks            
            //assign color
            int color_id = (last_color_id+1)%256;
            last_color_id = color_id;
            printf("Assigned color id: %d\n", color_id);
            mask_colors->push_back(color_id);
            GLuint newGLTexture = createGLTexture(mask_rgb, GL_RGBA);
            maskTextures->push_back(newGLTexture);
            storedMasks.push_back(mask);

            printf("Added mask\n");
            //glGenBuffers(1, &newGLTexture);
            //printf("%u\n", newGLTexture);
        } else {
			//If the mask is already in storedMasks we will delete it
            printf("Should delete mask %d ", pos);
            masksToDelete.push_back(pos);            
            printf("Mask already exist\n");
        }
        break; //just add 1
    }
    for(int& i : masksToDelete) {
        storedMasks.erase(storedMasks.begin() + i);
        maskTextures->erase(maskTextures->begin() + i);
        mask_colors->erase(mask_colors->begin() + i);
    }


}*/

void debugInspectImage(cv::Mat &input_image); //from simplify

//Computes the texture of the mask and it's simplified version
void compute_mask_textures(Mask & aMask, int R, int G, int B) {
    printf("compute_mask_textures R,G,B = %d, %d, %d\n", R, G, B);
    sam_image_u8 mask_rgba_binary = sam_mask_to_sam_4channels(aMask.samMask, 180);
    GLuint newGLTexture = createGLTexture(mask_rgba_binary, GL_RGBA);
    aMask.maskTexture = newGLTexture;

    //simplified (
    cv::Mat input_image_opencv;
    sam_image_grayscale2opencv(aMask.samMask, input_image_opencv);//Does initialize the result
    
    
    //input_image_opencv = 1-channel image
    cv::Mat output_image_opencv_bgra = cv::Mat::zeros(input_image_opencv.size(), CV_8UC4);
    //This one does not initialize the result. From simplify.cpp
    //The color is added here
    aMask.simplifiedContours = simplifyColorSegment(input_image_opencv, output_image_opencv_bgra, false, R, G, B); 
    //cv::imshow("image", output_image_opencv_bgra);
    //debugInspectImage(output_image_opencv_bgra);


    /*
    sam_image_u8 mask_simplified;
    opencv_image2sam_binarymask(mask_simplified, output_image_opencv);
    sam_image_u8 mask_simplified_rgb = sam_mask_to_sam_4channels(mask_simplified, 255);
    //mask_simplified_rgb = 4-channels but all with same value (1 or 0)
    //No color information here, color will be applied when draw_list->AddImage...
    GLuint newGLTextureSimplified = createGLTexture(mask_simplified_rgb, GL_RGBA);
    aMask.simplifiedMaskTexture = newGLTextureSimplified;
    */

    
    //DEBUG
    //printf("R,G,B = %.2lf, %.2lf, %.2lf\n", R, G, B);
    //sam_image_u8 mask_simplified_rgb;
    //opencv_image2sam(mask_simplified_rgb, output_image_opencv_bgra);
    //opencv_image4channels_to_sam4channels(mask_simplified_rgb, output_image_opencv_bgra);

    //GLuint newGLTextureSimplified = createGLTexture(mask_simplified_rgb, GL_RGB);
    GLuint newGLTextureSimplified = createGLTextureOpenCV(output_image_opencv_bgra, GL_RGBA);
    
        

    aMask.simplifiedMaskTexture = newGLTextureSimplified;
    aMask.opencv_mask_simplified = output_image_opencv_bgra;
    //
    

    aMask.textures_computed = true;
}

//Version which puts color into the mask
//Still does not work
/*void compute_mask_textures(Mask & aMask, int R, int G, int B) {
    sam_image_u8 mask_rgb = sam_mask_to_sam_4channels(aMask.samMask, 180);
    GLuint newGLTexture = createGLTexture(mask_rgb, GL_RGBA);
    aMask.maskTexture = newGLTexture;
    cv::Mat input_image_opencv;
    sam_image_grayscale2opencv(aMask.samMask, input_image_opencv);//Does initialize the result
    //input_image_opencv = 1-channel image
    cv::Mat output_image_opencv = cv::Mat::zeros(input_image_opencv.size(), CV_8UC4);
    //This one does not initialize the result. From simplify.cpp
    aMask.simplifiedContours = simplifyColorSegment(input_image_opencv, output_image_opencv, false, R, G, B);  
    sam_image_u8 mask_simplified_rgb;
    opencv_image4channels_to_sam4channels(mask_simplified_rgb, output_image_opencv);
    GLuint newGLTextureSimplified = createGLTexture(mask_simplified_rgb, GL_RGBA);
    aMask.simplifiedMaskTexture = newGLTextureSimplified;
    aMask.textures_computed = true;
}*/

void compute_mask_textures_all_frames(std::vector<Frame> & frames) 
{
    int numFrames = frames.size();
    int f = 0;
    //iterate through all frames 
    //assumes that the first frame has been already computed
    for (Frame & aFrame : frames) {
        printf("PROCESSING TEXTURES FROM FRAME %d \n", f);
        //iterate through all the masks of the frame
        for (Mask & aMask : aFrame.masks) {
            printf("\tPROCESSING MASK...\n", f);
            if (aMask.mask_computed && !aMask.textures_computed) {
                printf("\t\tcompute_mask_textures...\n", f);
                printf("compute_mask_textures_all_frames R,G,B = %d, %d, %d\n", aMask.color[0]*255, aMask.color[1]*255, aMask.color[2]*255);
                compute_mask_textures(aMask, aMask.color[0]*255, aMask.color[1]*255, aMask.color[2]*255);
                printf("\t\tcomputed textures...\n", f);
            }
        }
        f++;
    }
}

//called by the editor in frameWindow() and if clicked
//computes the masks at the given point and checks if it's a new one
//currently the passed storedMasks are just the masks of one mask
//void compute_masks(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y, std::vector<sam_image_u8> & storedMasks, std::vector<int> * mask_colors, int & last_color_id, int R, int G, int B, std::vector<GLuint> *simplifiedMaskTextures) {
//void compute_mask_and_textures(Frame & aFrame, const sam_params & params, sam_state & state, int x, int y, int R, int G, int B, MyState &myState) {
void compute_mask_and_textures(Frame & aFrame, int x, int y, int R, int G, int B, MyState &myState) {
    printf("compute_masks with selected_mask = %d\n", myState.selected_mask);
    sam_image_u8 mask;
    bool maskFound;
    //maskFound = get_best_sam_mask_at_point(x, y, aFrame.img_sam_format, state, params.n_threads, mask); 
    maskFound = myState.segmentor.get_best_mask_at_point(x, y, aFrame.img_sam_format, mask); 
    printf("COMPUTED MASK (%d,%d) from (%d,%d): \n", aFrame.img_sam_format.nx, aFrame.img_sam_format.ny, mask.nx, mask.ny);
        

    if (maskFound) {        
        //printf("isEmptyMaskDEBUG(mask)=%d\n", isEmptyMaskDEBUG(mask));
        int pos = masks_already_in_list(mask, aFrame);
        
        if (pos == -1) { //the mask is new, not in storedMasks  
            printf("the mask is new\n");
            //If there's a selected mask we will replace it
            //if not we will create a new mask
            Mask *targetMask;
            if (myState.selected_mask == -1) { 
                Mask newMask;
                targetMask = &newMask;  
            } else {
                targetMask = &aFrame.masks[myState.selected_mask];
            }    
            //Mask newMask;
            targetMask->samMask = mask;
            targetMask->mask_computed = true;
            targetMask->mask_computed_at_x = x;
            targetMask->mask_computed_at_y = y;
            printf("compute_mask_center...\n");
            compute_mask_center(*targetMask);//from common1.c
            printf("compute_mask_textures...\n");
            printf("compute_mask_and_textures R,G,B = %d, %d, %d\n", R, G, B);

            //compute_mask_textures(*targetMask, R, G, B);
            compute_mask_textures(*targetMask, targetMask->color[0]*256, targetMask->color[1]*256, targetMask->color[2]*256);
            //aFrame.newMask(newMask); 
            if (myState.selected_mask == -1) {  
                printf("aFrame.newMask...\n");     
                aFrame.newMask(*targetMask);
                printf("Added mask with id=%d\n", targetMask->maskId);
            } else {
                printf("Changed mask with id=%d\n", targetMask->maskId);
            }


        } else { //the mask already exists (and is not empty)
            //OLD: If the mask is already in storedMasks we will delete it
            //NOW: If the mask is already in storedMasks we will mark is as not computed
            /*
            printf("Deleting mask %d ", pos);
            aFrame.masks.erase(aFrame.masks.begin() + pos);
            //If we have erased the selected mask let's deselect
            if (pos == myState.selected_mask)
                myState.selected_mask = -1;
            */
            aFrame.masks[pos].mask_computed = false;
            /*if (pos == myState.selected_mask)
                myState.selected_mask = -1;*/
        }
    }
}

//computes the masks at the given point and checks if it's a new one
//currently the passed storedMasks are just the masks of one mask
//void compute_masks(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y, std::vector<sam_image_u8> & storedMasks, std::vector<int> * mask_colors, int & last_color_id, int R, int G, int B, std::vector<GLuint> *simplifiedMaskTextures) {
void compute_mask_and_texturesOLD(Frame & aFrame, const sam_params & params, sam_state & state, int x, int y, int R, int G, int B, MyState &myState) {
    printf("compute_masks with selected_mask = %d\n", myState.selected_mask);
    sam_image_u8 mask;
    bool maskFound;
    maskFound = get_best_sam_mask_at_point(x, y, aFrame.img_sam_format, state, params.n_threads, mask); 
    if (maskFound) {        
        //printf("isEmptyMaskDEBUG(mask)=%d\n", isEmptyMaskDEBUG(mask));
        int pos = masks_already_in_list(mask, aFrame);
        
        if (pos == -1) { //the mask is new, not in storedMasks  
            printf("the mask is new\n");
            //If there's a selected mask we will replace it
            //if not we will create a new mask
            Mask *targetMask;
            if (myState.selected_mask == -1) { 
                Mask newMask;
                targetMask = &newMask;  
            } else {
                targetMask = &aFrame.masks[myState.selected_mask];
            }    
            //Mask newMask;
            targetMask->samMask = mask;
            targetMask->mask_computed = true;
            targetMask->mask_computed_at_x = x;
            targetMask->mask_computed_at_y = y;
            printf("compute_mask_center...\n");
            compute_mask_center(*targetMask);//from common1.c
            printf("compute_mask_textures...\n");
            compute_mask_textures(*targetMask, R, G, B);
            //aFrame.newMask(newMask); 
            if (myState.selected_mask == -1) {  
                printf("aFrame.newMask...\n");     
                aFrame.newMask(*targetMask);
                printf("Added mask with id=%d\n", targetMask->maskId);
            } else {
                printf("Changed mask with id=%d\n", targetMask->maskId);
            }

        } else { //the mask already exists (and is not empty)
            //OLD: If the mask is already in storedMasks we will delete it
            //NOW: If the mask is already in storedMasks we will mark is as not computed
            /*
            printf("Deleting mask %d ", pos);
            aFrame.masks.erase(aFrame.masks.begin() + pos);
            //If we have erased the selected mask let's deselect
            if (pos == myState.selected_mask)
                myState.selected_mask = -1;
            */
            aFrame.masks[pos].mask_computed = false;
            /*if (pos == myState.selected_mask)
                myState.selected_mask = -1;*/
        }
    }
}


//Called from the editor/frameWindow si myState.clicked 
void simplify_segmented_frame(MyState &myState) 
{
    //takes all the masks from myState.aVideo.frames[myState.selected_frame].masks
    //, simplify everything and
    //generates myState.aVideo.frames[myState.selected_frame].tex_simplified

    Frame & aFrame = myState.aVideo.frames[myState.selected_frame];
    //cv::Mat input_image_opencv = aFrame.img;

    cv::Mat output_image_opencv_bgra = cv::Mat::zeros(cv::Size(aFrame.img_sam_format.nx,aFrame.img_sam_format.ny), CV_8UC4);
    for (auto& aMask : aFrame.masks) {
        if (aMask.mask_computed && aMask.visible) {

            //cv::imshow("image", output_image_opencv_bgra);
            printf("OVERLAYING (%d,%d) over (%d,%d): \n", aMask.opencv_mask_simplified.cols, aMask.opencv_mask_simplified.rows, output_image_opencv_bgra.cols, output_image_opencv_bgra.rows);
            //aMask.simplifiedContours = simplifyColorSegment(input_image_opencv, output_image_opencv_bgra, false, R, G, B);   
            
            overlay(output_image_opencv_bgra, output_image_opencv_bgra, aMask.opencv_mask_simplified);
            //output_image_opencv_bgra = output_image_opencv_bgra + aMask.opencv_mask_simplified;


            //DEBUG: output_image_opencv_bgra = output_image_opencv_bgra + aMask.opencv_mask_simplified;
        }
    }
    addRimLight(output_image_opencv_bgra, output_image_opencv_bgra, 5);
    GLuint newGLTextureSimplified = createGLTextureOpenCV(output_image_opencv_bgra, GL_RGBA);
    
    printf("DEBUG aFrame.tex_simplified = newGLTextureSimplified;\n");
    aFrame.tex_simplified = newGLTextureSimplified;
    //DEBUG
    //aFrame.tex_simplified = aFrame.tex;
}


/*
//computes the masks at the given point
//currently the passed storedMasks are just the masks of one mask
void compute_masks(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y, std::vector<sam_image_u8> & storedMasks, std::vector<int> * mask_colors, int & last_color_id) {
    printf("compute_masks\n");
    std::vector<sam_image_u8> masks;
    //std::vector<GLuint> maskTextures;
    //float x = 100.f;
    //float y = 100.f;
    sam_point pt { static_cast<float>(x), static_cast<float>(y)};
    printf("pt = (%f, %f)\n", pt.x, pt.y);

    masks = sam_compute_masks(img, params.n_threads, pt, state);

    std::vector<int> masksToDelete;
    for (auto& mask : masks) {
        sam_image_u8 mask_rgb = { mask.nx, mask.ny, };
        mask_rgb.data.resize(4*mask.nx*mask.ny);
        for (int i = 0; i < mask.nx*mask.ny; ++i) {
            mask_rgb.data[4*i+0] = mask.data[i];
            mask_rgb.data[4*i+1] = mask.data[i];
            mask_rgb.data[4*i+2] = mask.data[i];
            mask_rgb.data[4*i+3] = mask.data[i];
        }
        int pos = masks_already_in_list(mask, storedMasks);
        
        if (pos == -1) { //the mask is new, not in storedMasks            
            //assign color
            int color_id = (last_color_id+1)%256;
            last_color_id = color_id;
            printf("Assigned color id: %d\n", color_id);
            mask_colors->push_back(color_id);
            GLuint newGLTexture = createGLTexture(mask_rgb, GL_RGBA);
            maskTextures->push_back(newGLTexture);
            storedMasks.push_back(mask);

            printf("Added mask\n");
            //glGenBuffers(1, &newGLTexture);
            //printf("%u\n", newGLTexture);
        } else {
            //If the mask is already in storedMasks we will delete it
            printf("Should delete mask %d ", pos);
            masksToDelete.push_back(pos);            
            printf("Mask already exist\n");
        }
        break; //just add 1
    }
    for(int& i : masksToDelete) {
        storedMasks.erase(storedMasks.begin() + i);
        maskTextures->erase(maskTextures->begin() + i);
        mask_colors->erase(mask_colors->begin() + i);
    }


}
*/