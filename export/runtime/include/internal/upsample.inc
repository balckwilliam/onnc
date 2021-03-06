void nearest_upsample(int32_t output_Y_ndim, const int32_t* restrict output_Y_dims,
                      const int32_t* restrict output_convs, const int32_t* restrict input_convs, float* restrict scales,
                      const float* restrict input_X, float* restrict output_Y)
{
  int32_t output_size = output_convs[0] * output_Y_dims[0];
  for (int32_t i = 0; i < output_size; ++i) {
    // Convert output offset to input coordination
    int32_t input_coord[output_Y_ndim];
    for (int32_t j = 0, output_offset = i; j < output_Y_ndim; ++j) {
      input_coord[j] = output_offset / output_convs[j] / scales[j];
      output_offset %= output_convs[j];
    }
    // Convert input coordination to input offset
    int32_t input_offset = 0;
    for (int32_t j = 0; j < output_Y_ndim; ++j) {
      input_offset += input_coord[j] * input_convs[j];
    }
    output_Y[i] = input_X[input_offset];
  }
}

#include "upsample.impl.inc"
