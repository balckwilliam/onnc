void ONNC_RUNTIME_globallppool_float(void* restrict onnc_runtime_context, const float* restrict input_X,
                                     int32_t input_X_ndim, const int32_t* restrict input_X_dims,
                                     float* restrict output_Y, int32_t output_Y_ndim,
                                     const int32_t* restrict output_Y_dims, int32_t p)
{
  int32_t mul = 1;
  for (int32_t i = input_X_ndim - 1; i >= 2; --i) {
    mul *= input_X_dims[i];
  }
  for (int32_t i = 0; i < output_Y_dims[0]; ++i) {
    for (int32_t j = 0; j < output_Y_dims[1]; ++j) {
      output_Y[i * output_Y_dims[1] + j] = 0;
      for (int32_t k = 0; k < mul; ++k) {
        output_Y[i * output_Y_dims[1] + j] += powf(fabs(input_X[(i * input_X_dims[1] + j) * mul + k]), p);
      }
      output_Y[i * output_Y_dims[1] + j] = powf(output_Y[i * output_Y_dims[1] + j], 1.0 / p);
    }
  }
}
