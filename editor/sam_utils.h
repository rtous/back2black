#ifndef __SAM_UTILS_H__
#define __SAM_UTILS_H__

#include <string>
#include "sam.h"
#include "common1.h"
#include "state.h"

void set_params(sam_params * params);
//void compute_mask_and_textures(Frame & aFrame, const sam_params & params, sam_state & state, int x, int y, int R, int G, int B, MyState &myState);
void compute_mask_and_textures(Frame & aFrame, int x, int y, int R, int G, int B, MyState &myState);
void compute_mask_textures_all_frames(std::vector<Frame> & frames);
void simplify_segmented_frame(MyState &myState);
void compute_mask_textures(Mask & aMask, int R, int G, int B);

#endif
