#ifndef __UTILS_H__
#define __UTILS_H__

static bool get_screen_size(SDL_DisplayMode &dm, SDL_Window* window);
static sam_image_u8 downscale_img(sam_image_u8 &img , float scale);
static bool downscale_img_to_screen(sam_image_u8 &img, SDL_Window* window);
GLuint createGLTexture(const sam_image_u8 & img, GLint format);
void enable_blending(const ImDrawList*, const ImDrawCmd*);
void disable_blending(const ImDrawList*, const ImDrawCmd*);

#endif
