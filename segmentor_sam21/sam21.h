#ifndef __SAM21_H__
#define __SAM21_H__


#include <stdio.h>  
#include "onnxruntime_utils.h" 
#include "onnxruntime_utils.h" 
#include "tensor_utils.h"

struct PromptPoint {
    float x;   // in original frame space
    float y;
    int label; // 1=positive, 0=negative
};

//outputs of the image encoder to be stored 
//in a vector
struct FrameEmbedding {
    TensorCopy vision_features;
    TensorCopy backbone_fpn_0;
    TensorCopy backbone_fpn_1;
    TensorCopy dense_pe;
};



class InferenceState {
public:
    //std::vector<Ort::Value> img_encoder_out;
    TensorCopy vision_features;
    TensorCopy backbone_fpn_0;
    TensorCopy backbone_fpn_1;
    TensorCopy dense_pe;

    TensorCopy prompt_encoder_out_dense_pe;
    TensorCopy prompt_encoder_out_sparse_embeddings;
    TensorCopy prompt_encoder_out_dense_embeddings;

    // Output of obj_ptr_tpos_proj_hiera
    //TensorCopy obj_ptr_tpos_proj_hiera_out_x_out;
    std::vector<Ort::Value> obj_ptr_tpos_proj_hiera_out;

    // --- memory bank (per-frame) ---
    std::vector<TensorCopy> maskmem_features_BC64x64; // [1,64,64,64] per saved frame (vision_features)
    std::vector<TensorCopy> maskmem_posenc_BC64x64;   // [1,64,64,64] per saved frame (vision_pos_enc)
   
    //tracked_mask_head (does not need to be recomputed after the first frame)
    int maxScoreIdx;
   };

class SAM21 {      
  public:  
    //I use std::optional, +C++17, to avoid initializing class members in the constructor header
    Ort::MemoryInfo memory_info;
    OrtModel img_encoder;
    OrtModel prompt_encoder;
    OrtModel img_decoder;
    OrtModel mlp;
    OrtModel mem_encoder;
    OrtModel mem_attention;
    OrtModel obj_ptr_tpos_proj_hiera;
    
    InferenceState inference_state;

    //Optional, to save all the embeddings first (for later inference many times)
    std::vector<FrameEmbedding> video_frames_embeddings;

    SAM21(std::string path, std::string model_type);
    void test();
    void image_encoder(cv::Mat image);
    //WARNING: for a single image call inference_frame with keep_memory=false
    //         for a video cannot reuse a first frame called with keep_memory=false
    //         need to call inference_frame with keep_memory=true from frame 0
    cv::Mat inference_frame(cv::Mat image, int frame_num, std::vector<PromptPoint> promptPoints, bool keep_memory, bool use_precoputed_embedding_vector);
    void reset_memory();
};

#endif