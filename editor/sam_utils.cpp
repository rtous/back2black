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
#include "common1.h"
#include "simplify.h"


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

int masks_already_in_list(sam_image_u8 candidateMask, std::vector<sam_image_u8> masks) {
    int mask_count = 0;
    printf("Checking if mask exists.\n");
    for (auto& mask : masks) {
        int i;
        float pixels_1_any_of_both_masks = 0;
        float coincidences = 0;
        for (i = 0; i < mask.nx*mask.ny; ++i) {
            if (candidateMask.data[i] != 0 || mask.data[i] != 0) {
                pixels_1_any_of_both_masks++;
                if (candidateMask.data[i] == mask.data[i])
                    coincidences++;
            }
        }
        printf("coincidences = %f percent", coincidences/pixels_1_any_of_both_masks);
        if (coincidences/pixels_1_any_of_both_masks > 0.75)
            return mask_count;
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

//computes the masks at the given point
//currently the passed storedMasks are just the masks of one object
void compute_masks(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y, std::vector<sam_image_u8> & storedMasks, std::vector<int> * mask_colors, int & last_color_id, int R, int G, int B, std::vector<GLuint> *simplifiedMaskTextures) {
    printf("compute_masks\n");
    
    sam_image_u8 mask;
    get_best_sam_mask_at_point(x, y, img, state, params.n_threads, mask);
    
    std::vector<int> masksToDelete;
    
    /*sam_image_u8 mask_rgb = { mask.nx, mask.ny, };
    mask_rgb.data.resize(4*mask.nx*mask.ny);
    for (int i = 0; i < mask.nx*mask.ny; ++i) {
        mask_rgb.data[4*i+0] = mask.data[i];
        mask_rgb.data[4*i+1] = mask.data[i];
        mask_rgb.data[4*i+2] = mask.data[i];
        mask_rgb.data[4*i+3] = mask.data[i];
    }*/
    

    int pos = masks_already_in_list(mask, storedMasks);
    
    if (pos == -1) { //the mask is new, not in storedMasks            
        //assign color
        int color_id = (last_color_id+1)%256;
        last_color_id = color_id;
        printf("Assigned color id: %d\n", color_id);
        mask_colors->push_back(color_id);
        sam_image_u8 mask_rgb = sam_image2color(mask);
        GLuint newGLTexture = createGLTexture(mask_rgb, GL_RGBA);
        maskTextures->push_back(newGLTexture);
        storedMasks.push_back(mask);

        //Simplify:
        cv::Mat input_image_opencv;
        sam_image2opencv(mask, input_image_opencv);//Does initialize the result
        cv::Mat output_image_opencv = cv::Mat::zeros(input_image_opencv.size(), CV_8UC4);
        //This one does not initialize the result
        simplifyColorSegment(input_image_opencv, output_image_opencv, false, R, G, B); 
        sam_image_u8 mask_simplified;
        //opencv_image2sam(mask_simplified_rgb, output_image_opencv);
        opencv_image2sam_binarymask(mask_simplified, output_image_opencv);
        sam_image_u8 mask_simplified_rgb = sam_image2color(mask_simplified);
        GLuint newGLTextureSimplified = createGLTexture(mask_simplified_rgb, GL_RGBA);
        simplifiedMaskTextures->push_back(newGLTextureSimplified);



        //cv::imwrite("output/delete.png", output_image_opencv);
        //cv::imwrite("output/delete.png", output_image_opencv);

        //GLuint newGLTexture = createGLTextureFromO(mask_rgb, GL_RGBA);

        printf("Added mask\n");
        //glGenBuffers(1, &newGLTexture);
        //printf("%u\n", newGLTexture);
    } else {
        //If the mask is already in storedMasks we will delete it
        printf("Should delete mask %d ", pos);
        masksToDelete.push_back(pos);            
        printf("Mask already exist\n");
    }
    
    for(int& i : masksToDelete) {
        storedMasks.erase(storedMasks.begin() + i);
        maskTextures->erase(maskTextures->begin() + i);
        simplifiedMaskTextures->erase(simplifiedMaskTextures->begin() + i);
        mask_colors->erase(mask_colors->begin() + i);
    }


}

/*
//computes the masks at the given point
//currently the passed storedMasks are just the masks of one object
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