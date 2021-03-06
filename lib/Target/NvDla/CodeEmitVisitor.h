//===- CodeEmitVisitor.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_CODE_EMIT_VISITOR_H
#define TARGET_NVDLA_CODE_EMIT_VISITOR_H
#include "Compute/NvDlaConvRelu.h"
#include "Compute/NvDlaConvReluMaxPool.h"
#include "Compute/NvDlaGemmRelu.h"
#include "NvDlaComputeVisitor.h"
#include "NvDlaDefine.h"
#include "NvDlaMeta.h"

#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/CustomVisitor.h>

namespace onnc {
namespace nvdla {

class CodeEmitVisitor : public NvDlaComputeVisitor, private NvDlaConstants
{
public:
  CodeEmitVisitor(const NvDlaConstants& constants, NvDlaBackendMeta& meta) noexcept
    : NvDlaConstants{constants}
    , m_pMeta{meta}
  {}

  void visit(const Initializer& pInitializer) override;
  void visit(const InputOperator& pInputOperator) override;
  void visit(const OutputOperator& pOutputOperator) override;

  void visit(const Conv& pConv) override;
  void visit(const Reshape& pReshape) override;
  void visit(const Relu& pRelu) override;
  void visit(const LRN& pLRN) override;
  void visit(const MaxPool& pMaxPool) override;
  void visit(const AveragePool& pAveragePool) override;
  void visit(const Gemm& pGemm) override;
  void visit(const Softmax& pSoftmax) override;
  void visit(const Concat& pConcat) override;
  void visit(const NvDlaConvRelu& pConvRelu) override;
  void visit(const NvDlaGemmRelu& pGemmRelu) override;
  void visit(const NvDlaConvReluMaxPool& pConvReluMaxPool) override;
  void visit(const NvDlaMaxPool& pMaxPool) override;
  void visit(const Sum& pSum) override;

  void conv(const Tensor* x, const Tensor* w, const Tensor* b, const Tensor* y);
  int  packWeight(const Tensor* t, int dims[4], int gidx, bool shouldPadZero = false);
  int  packBias(const ComputeOperator& co, const Tensor* t, int dims[4], int gidx);

  void issueEmuOp(NvDlaEmuOperation* op);
  int  issueEmuAddr(int mid);
  void issueDlaOp(NvDlaDlaOperation* op, NvDlaDlaOperation* op_fuse, NvDlaDlaOperation* op_prev);
  int  issueDlaAddr(int mid, const NvDlaCubeInfo& cube, int groups, int gidx, int hofs);

private:
  NvDlaBackendMeta& m_pMeta;
};

} // namespace nvdla
} // namespace onnc

#endif
