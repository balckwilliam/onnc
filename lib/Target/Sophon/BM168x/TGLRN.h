//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#ifndef ONNX_BM_TGLRN_H
#define ONNX_BM_TGLRN_H

#include "ComputeOperator.h"
#include <onnx/common/ir.h>

// m_MemOperands: input, output
namespace onnc {

class TGLRN : public ComputeOperator2
{
public:
  TGLRN(const ::onnx::Node &pNode);

  void emit() const override;

private:
  int m_N;
  int m_C;
  int m_H;
  int m_W;
  float m_Alpha;
  float m_Beta;
  // bias
  float m_K;
  int m_LocalSize;
};

} // namespace onnc

#endif