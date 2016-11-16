// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Precompiled Headers]
#if defined(FOG_PRECOMP)
#include FOG_PRECOMP
#endif // FOG_PRECOMP

// [Dependencies]
#include <Fog/Core/Threading/Lock.h>
#include <Fog/Core/Threading/Thread.h>
#include <Fog/Core/Tools/Logger.h>
#include <Fog/G2d/OS/WinGdiPlus.h>

namespace Fog {

// ===========================================================================
// [Fog::WinGdiPlus]
// ===========================================================================

WinGdiPlus::WinGdiPlus() :
  _gdipToken((ULONG_PTR)NULL)
{
  static const char symbolNames[] =
    "GdiplusStartup\0"
    "GdiplusShutdown\0"
    "GdipLoadImageFromStream\0"
    "GdipSaveImageToStream\0"
    "GdipDisposeImage\0"
    "GdipGetImageType\0"
    "GdipGetImageWidth\0"
    "GdipGetImageHeight\0"
    "GdipGetImageFlags\0"
    "GdipGetImagePixelFormat\0"
    "GdipGetImageGraphicsContext\0"
    "GdipImageGetFrameCount\0"
    "GdipImageSelectActiveFrame\0"
    "GdipCreateBitmapFromScan0\0"
    "GdipSetCompositingMode\0"
    "GdipDrawImageI\0"
    "GdipDrawImageRectI\0"
    "GdipFlush\0"
    "GdipDeleteGraphics\0"

    "GdipGetImageEncoders\0"
    "GdipGetImageEncodersSize\0"
    ;

  // Open the GdiPlus library and load all required symbols.
  if (_library.openLibrary(StringW::fromAscii8("gdiplus")) != ERR_OK)
  {
    Logger::error("Fog::WinGdiPlus", "WinGdiPlus",
      "Failed to load gdiplus.dll.");
    return;
  }

  const char* badSymbol;
  if (_library.getSymbols(_symbols, symbolNames, FOG_ARRAY_SIZE(symbolNames), NUM_SYMBOLS, (char**)&badSymbol) != NUM_SYMBOLS)
  {
    Logger::error("Fog::WinGdiPlus", "WinGdiPlus",
      "Failed to load symbol %s.", badSymbol);
    _library.close();
    return;
  }

  // Startup the GdiPlus library.
  GpGdiplusStartupInput startupInput;
  startupInput.GdiplusVersion = 1;
  startupInput.DebugEventCallback = NULL;
  startupInput.SuppressBackgroundThread = false;
  startupInput.SuppressExternalCodecs = false;

  GpStatus status = _GdiplusStartup(&_gdipToken, &startupInput, NULL);
  if (status != GpOk)
  {
    Logger::error("Fog::WinGdiPlus", "WinGdiPlus",
      "GdiplusStartup() failed (status=%u).", status);
    _library.close();
  }
}

WinGdiPlus::~WinGdiPlus()
{
  if (_gdipToken != (ULONG_PTR)NULL)
    _GdiplusShutdown(_gdipToken);
}

// ============================================================================
// [Fog::WinGdiPlus - Statics]
// ============================================================================

static WinGdiPlus* _WinGdiPlus_global;
static Static<WinGdiPlus> _WinGdiPlus_instance;

// TODO: Mini-Leak, should be destroyed by the application exit, atexit, or similar.
WinGdiPlus* WinGdiPlus::get()
{
  WinGdiPlus* p = AtomicCore<WinGdiPlus*>::get(&_WinGdiPlus_global);

  if (FOG_UNLIKELY((uintptr_t)p <= (uintptr_t)0x1))
  {
    if (AtomicCore<WinGdiPlus*>::cmpXchg(&_WinGdiPlus_global, NULL, (WinGdiPlus*)0x1))
    {
      p = _WinGdiPlus_instance.init();
      AtomicCore<WinGdiPlus*>::set(&_WinGdiPlus_global, p);
    }
    else
    {
      while ((p = AtomicCore<WinGdiPlus*>::get(&_WinGdiPlus_global)) == (WinGdiPlus*)0x1)
        Thread::yield();
    }
  }

_Done:
  return (p->_library.isOpen()) ? p : NULL;
}

} // Fog namespace
