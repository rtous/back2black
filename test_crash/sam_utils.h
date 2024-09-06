#ifndef __SAM_UTILS_H__
#define __SAM_UTILS_H__

#include <string>
#include "sam.h"

void set_params(sam_params * params);
void compute_masks(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y, std::vector<sam_image_u8> & storedMasks, std::vector<int> * mask_colors, int  &last_color_id);

#endif
