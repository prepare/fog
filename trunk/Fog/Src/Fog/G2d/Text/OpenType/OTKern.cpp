// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Dependencies]
#include <Fog/Core/Memory/MemMgr.h>
#include <Fog/Core/Memory/MemOps.h>
#include <Fog/Core/Tools/Logger.h>
#include <Fog/G2d/Text/OpenType/OTEnum.h>
#include <Fog/G2d/Text/OpenType/OTKern.h>

namespace Fog {

// ============================================================================
// [Fog::OTHead - Init / Destroy]
// ============================================================================

static void FOG_CDECL OTKern_destroy(OTKern* self)
{
  // This results in crash in case that destroy is called twice by accident.
  self->_destroy = NULL;
}

static err_t FOG_CDECL OTKern_init(OTKern* self)
{
  // --------------------------------------------------------------------------
  // [Init]
  // --------------------------------------------------------------------------

  const uint8_t* data = self->getData();
  uint32_t dataLength = self->getDataLength();

#if defined(FOG_OT_DEBUG)
  Logger::info("Fog::OTKern", "init", 
    "Initializing 'kern' table (%u bytes).", dataLength);
#endif // FOG_OT_DEBUG

  FOG_ASSERT_X(self->_tag == FOG_OT_TAG('k', 'e', 'r', 'n'),
    "Fog::OTKern::init() - Not a 'kern' table.");

  self->_destroy = (OTTableDestroyFunc)OTKern_destroy;

  // --------------------------------------------------------------------------
  // [Header]
  // --------------------------------------------------------------------------

  const OTKernHeader* header = self->getHeader();

  return ERR_OK;
}

// ============================================================================
// [Init / Fini]
// ============================================================================

FOG_NO_EXPORT void OTKern_init(void)
{
  OTApi& api = fog_ot_api;

  // --------------------------------------------------------------------------
  // [OTKern]
  // --------------------------------------------------------------------------
  
  api.otkern_init = OTKern_init;
}

} // Fog namespace
