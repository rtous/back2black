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


//Computes the texture of the mask and it's simplified version
void compute_mask_textures(Mask & aMask, int R, int G, int B) {
    sam_image_u8 mask_rgb = sam_mask_to_sam_4channels(aMask.samMask, 180);
    GLuint newGLTexture = createGLTexture(mask_rgb, GL_RGBA);
    aMask.maskTexture = newGLTexture;
    cv::Mat input_image_opencv;
    sam_image_grayscale2opencv(aMask.samMask, input_image_opencv);//Does initialize the result
    cv::Mat output_image_opencv = cv::Mat::zeros(input_image_opencv.size(), CV_8UC4);
    //This one does not initialize the result. From simplify.cpp
    aMask.simplifiedContours = simplifyColorSegment(input_image_opencv, output_image_opencv, false, R, G, B); 
    sam_image_u8 mask_simplified;
    opencv_image2sam_binarymask(mask_simplified, output_image_opencv);
    sam_image_u8 mask_simplified_rgb = sam_mask_to_sam_4channels(mask_simplified, 255);
    GLuint newGLTextureSimplified = createGLTexture(mask_simplified_rgb, GL_RGBA);
    aMask.simplifiedMaskTexture = newGLTextureSimplified;
    aMask.textures_computed = true;

}

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
                compute_mask_textures(aMask, aMask.color[0]*255, aMask.color[1]*255, aMask.color[2]*255);
                printf("\t\tcomputed textures...\n", f);
            }
        }
        f++;
    }
}

//computes the masks at the given point and checks if it's a new one
//currently the passed storedMasks are just the masks of one mask
//void compute_masks(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y, std::vector<sam_image_u8> & storedMasks, std::vector<int> * mask_colors, int & last_color_id, int R, int G, int B, std::vector<GLuint> *simplifiedMaskTextures) {
void compute_mask_and_textures(Frame & aFrame, const sam_params & params, sam_state & state, int x, int y, int R, int G, int B, MyState &myState) {
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