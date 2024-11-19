#ifndef __UTILS_H__
#define __UTILS_H__

#include <opencv2/opencv.hpp> 
#include "common1.h"

static bool get_screen_size(SDL_DisplayMode &dm, SDL_Window* window);
static sam_image_u8 downscale_img(sam_image_u8 &img , float scale);
static bool downscale_img_to_screen(sam_image_u8 &img, SDL_Window* window);
bool downscale_img_to_size(sam_image_u8 &img, int dw, int dh);
GLuint createGLTexture(const sam_image_u8 & img, GLint format);
GLuint createGLTextureOpenCV(const cv::Mat & opencv_img, GLint format);
void enable_blending(const ImDrawList*, const ImDrawCmd*);
void disable_blending(const ImDrawList*, const ImDrawCmd*);
void compute_facial_textures_all_frames(std::vector<Frame> & frames);

#endif
