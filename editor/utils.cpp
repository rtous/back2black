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

#include "sam.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

bool load_image_from_file(const std::string & fname, sam_image_u8 & img) {
    int nx, ny, nc;
    auto data = stbi_load(fname.c_str(), &nx, &ny, &nc, 3);
    if (!data) {
        fprintf(stderr, "%s: failed to load '%s'\n", __func__, fname.c_str());
        return false;
    }
    if (nc != 3) {
        fprintf(stderr, "%s: '%s' has %d channels (expected 3)\n", __func__, fname.c_str(), nc);
        return false;
    }

    img.nx = nx;
    img.ny = ny;
    img.data.resize(nx * ny * 3);
    memcpy(img.data.data(), data, nx * ny * 3);

    stbi_image_free(data);

    return true;
}

/*void test(const std::string & fname, sam_image_u8 & img) {

}*/