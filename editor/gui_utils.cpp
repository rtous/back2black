/*
 * Utils mainly related to graphical aspects (though can involve sam-related stuff) 
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
#include "common1.h"


/*
#include "sam.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
*/

/**
 * Get the size of screen where the SDL window runs in.
 *
 * SDL_Window* window could be NULL, which means we get the screen size of the default 0-index display.
 * If window is not NULL, the we need to get the screen size of the display where the window runs in.
 *
 */
static bool get_screen_size(SDL_DisplayMode &dm, SDL_Window* window) {
    int displayIndex = 0;
    if (window != NULL) {
        displayIndex = SDL_GetWindowDisplayIndex(window);
    }
    if (displayIndex < 0) {
        return false;
    }
    if (SDL_GetCurrentDisplayMode(displayIndex, &dm) != 0) {
        return false;
    }

    fprintf(stderr, "%s: screen size (%d x %d) \n", __func__, dm.w, dm.h);
    return true;
}

// downscale image with nearest-neighbor interpolation
static sam_image_u8 downscale_img(sam_image_u8 &img , float scale) {
    sam_image_u8 new_img;

    int width = img.nx;
    int height = img.ny;

    int new_width = img.nx / scale + 0.5f;
    int new_height = img.ny / scale + 0.5f;

    new_img.nx = new_width;
    new_img.ny = new_height;
    new_img.data.resize(new_img.nx*new_img.ny*3);

    fprintf(stderr, "%s: scale: %f\n", __func__, scale);
    fprintf(stderr, "%s: resize image from (%d x %d) to (%d x %d)\n", __func__, img.nx, img.ny, new_img.nx, new_img.ny);

    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            int src_x = (x + 0.5f) * scale - 0.5f;
            int src_y = (y + 0.5f) * scale - 0.5f;

            int src_index = (src_y * width + src_x) * 3;
            int dest_index = (y * new_width + x) * 3;

            for (int c = 0; c < 3; ++c) {
                new_img.data[dest_index + c] = img.data[src_index + c];
            }
        }
    }


    return new_img;
}

/*static void scaleImage(MyState &myState) { //opencv tools
    aFrame.img//opencv
    Mat img_resized;
    //resize down
    cv::resize(aFrame.img, img_resized, cv::Size(down_width, down_height), INTER_LINEAR);
    
    opencv_image2sam(aFrame.img_sam_format, frame);
    aFrame.tex = createGLTexture(aFrame.img_sam_format, GL_RGB);
}*/

//my version of downscale_img_to_screen (need to unify both)
bool downscale_img_to_size(sam_image_u8 &img, int dw, int dh) {
    /*SDL_DisplayMode dm = {};
    if (!get_screen_size(dm, window)) {
        fprintf(stderr, "%s: failed to get screen size of the display.\n", __func__);
        return false;
    }
    fprintf(stderr, "%s: screen size (%d x %d) \n", __func__,dm.w,dm.h);
    if (dm.h == 0 || dm.w == 0) {
        // This means the window is running in other display.
        return false;
    }*/

    // Add 5% margin between screen and window
    //const float margin = 0.05f;
    //const int max_width  = dm.w - margin * dm.w;
    //const int max_height = dm.h - margin * dm.h;

    const int max_width  = dw;
    const int max_height = dh;

    fprintf(stderr, "%s: img size (%d x %d) \n", __func__,img.nx,img.ny);

    if (img.ny > max_height || img.nx > max_width) {
        fprintf(stderr, "%s: img size (%d x %d) exceeds maximum allowed size (%d x %d) \n", __func__,img.nx,img.ny,max_width,max_height);
        const float scale_y = (float)img.ny / max_height;
        const float scale_x = (float)img.nx / max_width;
        const float scale = std::max(scale_x, scale_y);

        img = downscale_img(img, scale);
    }

    return true;
}

static bool downscale_img_to_screen(sam_image_u8 &img, SDL_Window* window) {
    SDL_DisplayMode dm = {};
    if (!get_screen_size(dm, window)) {
        fprintf(stderr, "%s: failed to get screen size of the display.\n", __func__);
        return false;
    }
    fprintf(stderr, "%s: screen size (%d x %d) \n", __func__,dm.w,dm.h);
    if (dm.h == 0 || dm.w == 0) {
        // This means the window is running in other display.
        return false;
    }

    // Add 5% margin between screen and window
    const float margin = 0.05f;
    const int max_width  = dm.w - margin * dm.w;
    const int max_height = dm.h - margin * dm.h;

    fprintf(stderr, "%s: img size (%d x %d) \n", __func__,img.nx,img.ny);

    if (img.ny > max_height || img.nx > max_width) {
        fprintf(stderr, "%s: img size (%d x %d) exceeds maximum allowed size (%d x %d) \n", __func__,img.nx,img.ny,max_width,max_height);
        const float scale_y = (float)img.ny / max_height;
        const float scale_x = (float)img.nx / max_width;
        const float scale = std::max(scale_x, scale_y);

        img = downscale_img(img, scale);
    }

    return true;
}

GLuint createGLTextureOpenCV(const cv::Mat & opencv_img, GLint format) {
    //TODO: With this method, when used as a binary mask to apply colors
    //the colors are changed. So it's different from createGLTexture somehow???
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //image data
    cv::Mat img;
    cv::cvtColor(opencv_img, img, cv::COLOR_BGRA2RGBA);
    //cv::flip(img, img, -1);

    glTexImage2D(GL_TEXTURE_2D, 0, format, opencv_img.cols, opencv_img.rows, 0, format, GL_UNSIGNED_BYTE, img.data);

    return tex;
}

GLuint createGLTexture(const sam_image_u8 & img, GLint format) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, format, img.nx, img.ny, 0, format, GL_UNSIGNED_BYTE, img.data.data());

    return tex;
}

void enable_blending(const ImDrawList*, const ImDrawCmd*) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

void disable_blending(const ImDrawList*, const ImDrawCmd*) {
    glDisable(GL_BLEND);
}

//called
void compute_facial_textures_all_frames(std::vector<Frame> & frames) 
{
    int numFrames = frames.size();
    for (Frame & aFrame : frames) {
        if (aFrame.faces_computed && !aFrame.faces_textures_computed) {
            //GLuint newGLTextureFace = createGLTextureOpenCV(aFrame.faces, GL_RGBA);
            //aFrame.facesTexture = newGLTextureFace;
            //aFrame.faces_textures_computed = true;

            //With sam2texture (face)
            sam_image_u8 mask_simplified;
            opencv_image2sam_binarymask(mask_simplified, aFrame.faces);
            sam_image_u8 mask_simplified_rgb = sam_mask_to_sam_4channels(mask_simplified, 255);
            GLuint newGLTextureFace = createGLTexture(mask_simplified_rgb, GL_RGBA);
            aFrame.facesTexture = newGLTextureFace;
            
            //With sam2texture (eyes)
            sam_image_u8 mask_simplified2;
            opencv_image2sam_binarymask(mask_simplified2, aFrame.eyes);
            sam_image_u8 mask_simplified_rgb2 = sam_mask_to_sam_4channels(mask_simplified2, 255);
            GLuint newGLTextureFace2 = createGLTexture(mask_simplified_rgb2, GL_RGBA);
            aFrame.eyesTexture = newGLTextureFace2;
            
            aFrame.faces_textures_computed = true; 
        }
    }
}