set(SAM_LIB sam)

add_library(${SAM_LIB} sam.cpp sam.h)
target_link_libraries(${SAM_LIB} PUBLIC ggml)
target_include_directories(${SAM_LIB} PUBLIC .)
target_compile_features(${SAM_LIB} PUBLIC cxx_std_14)