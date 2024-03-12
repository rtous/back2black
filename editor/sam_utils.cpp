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
#include "utils.h"

/*void set_params(sam_params * params, std::string img_fname) {
    //./bin/editor -t 16 -i ../img.jpg -m ../checkpoints/ggml-model-f16.bin
    //params->seed = std::stoi(10);
    params->n_threads = 16;
    params->model = "checkpoints/ggml-model-f16.bin";
    params->fname_inp = img_fname;
    //params.fname_out = argv[++i];
}*/

void set_params(sam_params * params) {
    //./bin/editor -t 16 -i ../img.jpg -m ../checkpoints/ggml-model-f16.bin
    //params->seed = std::stoi(10);
    params->n_threads = 16;
    params->model = "checkpoints/ggml-model-f16.bin";
    //params->fname_inp = img_fname;
    //params.fname_out = argv[++i];
}

/*bool masks_already_in_list(sam_image_u8 mask, std::vector<sam_image_u8> masks) {
 return false;
}*/

bool masks_already_in_list(GLuint candidate_mask, std::vector<GLuint> *maskTextures) {
    for (auto& mask : *maskTextures) {
        if (candidate_mask == mask) return true;
    }
    return false;
}

void compute_masks(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y) {
    std::vector<sam_image_u8> masks;
    //std::vector<GLuint> maskTextures;
    //float x = 100.f;
    //float y = 100.f;
    sam_point pt { static_cast<float>(x), static_cast<float>(y)};
    printf("pt = (%f, %f)\n", pt.x, pt.y);

    masks = sam_compute_masks(img, params.n_threads, pt, state);

    /*if (!maskTextures->empty()) {
        glDeleteTextures(maskTextures->size(), maskTextures->data());
        maskTextures->clear();
    }*/

    for (auto& mask : masks) {
        sam_image_u8 mask_rgb = { mask.nx, mask.ny, };
        mask_rgb.data.resize(3*mask.nx*mask.ny);
        for (int i = 0; i < mask.nx*mask.ny; ++i) {
            mask_rgb.data[3*i+0] = mask.data[i];
            mask_rgb.data[3*i+1] = mask.data[i];
            mask_rgb.data[3*i+2] = mask.data[i];
        }
        GLuint newGLTexture = createGLTexture(mask_rgb, GL_RGB);
        if (!masks_already_in_list(newGLTexture, maskTextures)) {
            maskTextures->push_back(newGLTexture);
        }
    }
}