#ifndef __SAM_UTILS_H__
#define __SAM_UTILS_H__

#include <string>
#include "sam.h"
#include "common1.h"

void set_params(sam_params * params);
//void compute_masks(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y, std::vector<sam_image_u8> & storedMasks, std::vector<int> * mask_colors, int & last_color_id, int R, int G, int B, std::vector<GLuint> *simplifiedMaskTextures);
void compute_mask_and_textures(Frame & aFrame, const sam_params & params, sam_state & state, int x, int y, int R, int G, int B);
void compute_mask_textures_all_frames(std::vector<Frame> & frames);

#endif
