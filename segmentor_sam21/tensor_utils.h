#ifndef TENSOR_UTILS_H_
#define TENSOR_UTILS_H_

//WARNING: Remember that functions in a .h should be inline or you may find a duplicate symbol error during linking

#include <onnxruntime_cxx_api.h>

/*
    Ort::Value is move-only and represents ownership of the underlying tensor memory.
    When you assign (=) or insert into vector (push_back) you’re transfer ownership, the source becomes empty;
    So, if you want to reuse the same Ort::Value need to copy the contents in a TensorCopy.
    I use it, e.g., for the ouput of the prompt encoder (obtained just once from the first frame but used for every frame).
*/
class TensorCopy {
public:
  InputOutput info;
  std::vector<float> data; //bools will be stored as 1.0/0.0 
  int size;
};

//ready for floats and bools (not ints)
inline TensorCopy setTensorCopy(Ort::Value src) {
  TensorCopy tcopy;
  auto tensor_info = src.GetTensorTypeAndShapeInfo();
  std::vector<int64_t> shape = tensor_info.GetShape();
  auto src_size = tensor_info.GetElementCount();
  auto type = tensor_info.GetElementType();
  /*if (type != 1) {
    printf("ERROR: Trying to use setTensorCopy with a non-float tensor.\n");
    exit(-1);
  }*/
  tcopy.info.shape = shape;
  tcopy.info.type = type;
  tcopy.size = src_size;
  //std::vector<float> obj_ptrs(src_size); // first+recent // 16*256
  tcopy.data.resize(src_size);
  if (type == ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT) {
      const float* tensor_data = src.GetTensorData<float>();
      std::copy_n(tensor_data, src_size, std::begin(tcopy.data));
  } else if (type == ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL) {
      const bool* tensor_data = src.GetTensorData<bool>();
      for (size_t i = 0; i < tcopy.size; i++) {
        tcopy.data[i] = tensor_data[i] ? 1.0f : 0.0f;
      }
  } else {
    printf("ERROR: Trying to use setTensorCopy with a non-float or non-bool tensor.\n");
    exit(-1);
  }
  return tcopy;
}

//Don't need to change it at all.
inline Ort::Value getTensorCopy(TensorCopy& tcopy, Ort::MemoryInfo& memory_info) {
  if (tcopy.info.type == ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT) {
      auto tensor = Ort::Value::CreateTensor<float>(
                  memory_info,
                  tcopy.data.data(),
                  tcopy.data.size(),
                  tcopy.info.shape.data(),
                  tcopy.info.shape.size()
                  );
      return tensor;
  } else if (tcopy.info.type == ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL) {
      std::vector<uint8_t> tmp(tcopy.size);
      for (int i = 0; i < tcopy.size; ++i)
            tmp[i] = (tcopy.data[i] != 0.0f) ? uint8_t(1) : uint8_t(0);
      return Ort::Value::CreateTensor<bool>(
            memory_info,
            reinterpret_cast<bool*>(tmp.data()),
            tmp.size(),
            tcopy.info.shape.data(),
            tcopy.info.shape.size()
      );
  } else {
    printf("ERROR: Trying to use setTensorCopy with a non-float or non-bool tensor.\n");
    exit(-1);
  }
}



inline void printTensorCopyInfo(TensorCopy tcopy) {
  printf("num elements = [%d]\n", tcopy.size);
  printf("shape = [%s]\n", shape2string(tcopy.info.shape).c_str());
  printf("type = [%s]\n", OnnxTypeToString(tcopy.info.type));
  printf("10 elements = ");
  /*for(int i=0; i<10; i++) {
    printf("%f, ", tcopy.data[i]);
  }*/

}

//Used to reshape the outut of MLP
inline TensorCopy slice_1xNxC_toNxC(TensorCopy tcopy) {
  TensorCopy tcopy_out;
  tcopy_out.info.shape = tcopy.info.shape;
  tcopy_out.info.shape.erase(tcopy_out.info.shape.begin()); 
  tcopy_out.info.type = tcopy.info.type;
  tcopy_out.size = tcopy.size;
  tcopy_out.data.resize(tcopy_out.size);
  std::copy_n(tcopy.data.data(), tcopy_out.size, std::begin(tcopy_out.data));
  return tcopy_out;
}

// ---- NCHW -> [HW,1,C]
inline TensorCopy flatten_nchw_to_hw1c(TensorCopy& nchw) {
    // Expect [1,C,H,W]
    const auto& s = nchw.info.shape;
    int64_t C = s[1], H = s[2], W = s[3];

    TensorCopy out;
    out.info.type  = ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT;
    out.info.shape = { H * W, 1, C };
    out.size = H * W * C;
    out.data.resize(out.size);

    // NCHW (B=1): idx = ((c*H)+h)*W + w
    for (int64_t h = 0; h < H; ++h) {
        for (int64_t w = 0; w < W; ++w) {
            for (int64_t c = 0; c < C; ++c) {
                int64_t src_idx = (c * H + h) * W + w;
                int64_t dst_idx = (h * W + w) * C + c;           // [HW,C]
                out.data[dst_idx] = nchw.data[src_idx];
            }
        }
    }
    return out;
}

// ---- [HW,1,C] -> [1,C,64,64]
inline TensorCopy reshape_hw1c_to_nchw_64x64(TensorCopy& hw1c) {
    const auto& s = hw1c.info.shape;   // [HW,1,C]
    int64_t HW = s[0];
    int64_t C  = s[2];
    const int64_t H = 64, W = 64;
    // Optional sanity: if (HW != H*W) ...

    TensorCopy out;
    out.info.type  = ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT;
    out.info.shape = { 1, C, H, W };
    out.size = C * H * W;
    out.data.resize(out.size);

    for (int64_t h = 0; h < H; ++h) {
        for (int64_t w = 0; w < W; ++w) {
            for (int64_t c = 0; c < C; ++c) {
                int64_t src_idx = (h * W + w) * C + c;          // [HW,C]
                int64_t dst_idx = (c * H + h) * W + w;          // NCHW (B=1)
                out.data[dst_idx] = hw1c.data[src_idx];
            }
        }
    }
    return out;
}

// ---- concat a list of [-1,1,C] along axis 0
inline TensorCopy concat_seq_axis0(std::vector<TensorCopy>& xs) {
    TensorCopy out;

    if (xs.empty()) {
        // Return a minimal empty-like tensor with C=64 (common in this graph)
        out.info.type  = ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT;
        out.info.shape = { 0, 1, 64 };
        out.size = 0;
        return out;
    }

    int64_t C = xs[0].info.shape[2];
    int64_t totalL = 0;
    for (const auto& x : xs) {
        // Optionally validate x.info.shape[1]==1 and x.info.shape[2]==C
        totalL += x.info.shape[0];
    }

    out.info.type  = ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT;
    out.info.shape = { totalL, 1, C };
    out.size = totalL * C;
    out.data.resize(out.size);

    int64_t offsetL = 0;
    for (const auto& x : xs) {
        int64_t L = x.info.shape[0];
        // Copy L*C floats from x into out at row offset 'offsetL'
        std::memcpy(out.data.data() + offsetL * C, x.data.data(), sizeof(float) * L * C);
        offsetL += L;
    }
    return out;
}

// ---- make bool mask [len,1] (data stored as 1.0/0.0 in TensorCopy)
inline TensorCopy make_bool_mask(int64_t len, bool value) {
    TensorCopy out;
    out.info.type  = ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL;  // important
    out.info.shape = { len, 1 };
    out.size = len;
    out.data.resize(len);
    const float f = value ? 1.0f : 0.0f;
    std::fill(out.data.begin(), out.data.end(), f);
    return out;
}

#endif