#ifndef __SAM_UTILS_H__
#define __SAM_UTILS_H__

#include <string>
#include "sam.h"
#include "common1.h"
#include "state.h"

void set_params(sam_params * params);
//void compute_mask_and_textures(Frame & aFrame, const sam_params & params, sam_state & state, int x, int y, int R, int G, int B, MyState &myState);
void compute_mask_and_textures(Frame & aFrame, int x, int y, int R, int G, int B, MyState &myState);
void compute_mask_and_textures(Frame & aFrame, std::vector<MaskPoint> sam2_points, int R, int G, int B, MyState &myState);
//void compute_mask_textures_all_frames(std::vector<Frame> & frames, MyState &myState);
void compute_mask_textures_all_frames(std::vector<Frame> & frames, MyState &myState, bool simplify_and_color);
//void change_mask_color_all_frames(std::vector<Frame> & frames, MyState &myState, int maskId); 
void finishing_frame(MyState &myState, int frame_idx);
void finishing_all_frames(MyState &myState);
void compute_mask_textures(Mask & aMask, int R, int G, int B);

#endif
