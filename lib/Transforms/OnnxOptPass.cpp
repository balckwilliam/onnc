//===- OnnxOptPass.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Config/ONNX.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/ONNXWrapper/ONNXWrapper.h>
#include <onnc/Transforms/OnnxOptPass.h>

#include <onnx/optimizer/optimize.h>

#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>

using namespace onnc;

//===----------------------------------------------------------------------===//
// OnnxOptPass
//===----------------------------------------------------------------------===//
namespace onnc {
namespace internal {
  const char* getOptionName(OnnxOptPass::Option option) {
    static const char* names[] = {
      "extract_constant_to_initializer",
      "fuse_add_bias_into_conv",
      "fuse_bn_into_conv",
      "fuse_consecutive_squeezes",
      "fuse_consecutive_transposes",
      "fuse_transpose_into_gemm",
      "eliminate_identity",
      "eliminate_nop_pad",
      "eliminate_nop_transpose",
      "eliminate_unused_initializer",
    };

    const std::size_t idx = static_cast<std::size_t>(option);
    assert(idx < std::distance(std::begin(names), std::end(names)));

    return names[idx];
  }
} // namespace internal
} // namespace onnc

Pass::ReturnType OnnxOptPass::runOnModule(Module &pModule)
{
  if (names.empty()) {
    return Pass::kModuleNoChanged;
  }

  onnxInferShape(pModule);

  xProto mp;
  onnc::ExportModelProto(mp, pModule);
  mp = onnx::optimization::Optimize(
    mp,
    std::vector<std::string>{begin(names), end(names)}
  );
  pModule.delegate(xImportModelProto(mp));

  return Pass::kModuleChanged;
}

OnnxOptPass& OnnxOptPass::add(Option option)
{
  names.emplace(internal::getOptionName(option));
  return *this;
}
