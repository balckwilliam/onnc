//===- NvDlaMeta.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_META_H
#define TARGET_NVDLA_NVDLA_META_H

#include "NvDlaDefine.h"
#include "dla_interface.h"
#include "emu_interface.h"
#include "fp16.h"
#include "nvdla/ILoadable.h"
#include "priv/Loadable.h"
#include "priv/loadable_generated.h"

#include <onnc/IR/Compute/Tensor.h>

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iomanip>
#include <sstream>
#include <unordered_map>

#ifndef NDEBUG
#  define NVDLA_DBG(...) fprintf(stderr, __VA_ARGS__)
#else
#  define NVDLA_DBG(...) \
    do {                 \
    } while (false)
#endif

using namespace nvdla;
using namespace nvdla::priv;

namespace onnc {
struct concat_meta
{
  const Tensor* t;
  int           ofs;
};

typedef std::unordered_map<Value*, int>                  MemoryIdxTable;
typedef std::unordered_map<const Tensor*, const Tensor*> RemapTable;
typedef std::unordered_map<const Tensor*, concat_meta>   ConcatTable;

/** \class NvDlaDlaOperation
 *  \brief
 */
class NvDlaDlaOperation
{
public:
  NvDlaDlaOperation() noexcept;

public:
  struct dla_common_op_desc     op_dep;
  union dla_operation_container op_desc;
  union dla_surface_container   op_surf;
};

/** \class NvDlaEmuOperation
 *  \brief
 */
class NvDlaEmuOperation
{
public:
  NvDlaEmuOperation() noexcept;

public:
  union emu_operation_container        op_desc;
  union emu_operation_buffer_container op_buf;
};

class NvDlaBackendMeta : private NvDlaConstants
{
public:
  NvDlaBackendMeta(const NvDlaConstants& constants);

  ~NvDlaBackendMeta();

public:
  // memory allocation information for runtime (firmwares, memory buffer)
  std::vector<ILoadable::MemoryListEntry> m_MemoryListEntries;
  // addresses used in firmware
  std::vector<ILoadable::AddressListEntry> m_AddressListEntries;
  // input, output specific descriptor
  std::vector<ILoadable::TensorDescListEntry> m_TensorDescListEntries;
  // relocation information of input/output
  std::vector<ILoadable::RelocEntry> m_RelocEntries;

  // blobs, firmware binary (operators, initializer data)
  std::map<std::string, Loadable::Symbol> m_Symbols;
  // DLA or EMU batch tasks
  std::vector<ILoadable::TaskListEntry> m_TaskListEntries;
  // batch task submit order
  std::vector<ILoadable::SubmitListEntry> m_SubmitListEntries;
  // events between submits
  std::vector<ILoadable::EventListEntry> m_EventListEntries;

  int                             m_DlaAddresses;
  struct dla_network_desc         m_DlaNetworkDesc;
  int                             m_NumLUTs;
  std::vector<NvDlaDlaOperation*> m_DLAOperationList;
  std::vector<dla_lut_param*>     m_LUTList;
  NvDlaDlaOperation*              m_pDepOp[DLA_OP_NUM];
  NvDlaDlaOperation*              m_pPrevOp;

  emu_network_desc                m_EmuNetworkDesc;
  std::vector<NvDlaEmuOperation*> m_EMUOperationList;

  MemoryIdxTable m_MemIdxTable;
  RemapTable     m_ReshapeTable;
  ConcatTable    m_ConcatTable;

  int                                     m_NumBlobs;
  priv::LoadableFactory::LoadablePrivPair m_Loadable;
};

enum nvdla_cube_type
{
  NVDLA_CUBE_FEATURE,
  NVDLA_CUBE_WEIGHT,
  NVDLA_CUBE_IMAGE
};

class NvDlaCubeInfo : private NvDlaConstants
{
public:
  NvDlaCubeInfo(const NvDlaConstants& constants, nvdla_cube_type m, int n, int c, int h, int w);

  int getReducedBanks() const;

  void reduceBanks();

  ~NvDlaCubeInfo() = default;

public:
  nvdla_cube_type mode;
  int             dim_n;
  int             dim_c;
  int             dim_h;
  int             dim_w;
  int             eps;
  int             banks;
  int             size;
  int             stride_channel;
  int             stride_line;
  int             stride_surface;
  int             stride_plane;
  bool            reduced;
};

} // namespace onnc

#endif
