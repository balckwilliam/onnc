void ONNC_RUNTIME_randomnormallike_float(void* restrict onnc_runtime_context, const float* restrict input_input,
                                         int32_t input_input_ndim, const int32_t* restrict input_input_dims,
                                         float* restrict output_output, int32_t output_output_ndim,
                                         const int32_t* restrict output_output_dims, int32_t dtype, float mean,
                                         float scale, float seed)
{
  int32_t dataSize = 1;
  for (int32_t i = 0; i < input_input_ndim; i++) {
    dataSize *= input_input_dims[i];
  }

  srand(seed);
  for (int32_t i = 0; i < dataSize; i++) {
    output_output[i] = randomNormal(mean, scale);
  }
}
