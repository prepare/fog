// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_OS_WINUXTHEME_H
#define _FOG_G2D_OS_WINUXTHEME_H

// [Dependencies]
#include <Fog/Core/Global/Global.h>
#include <Fog/Core/Kernel/Object.h>
#include <Fog/Core/OS/Library.h>

// ============================================================================
// [Fog::WinUxTheme - Defs]
// ============================================================================

// #include <uxtheme.h>
typedef HANDLE HTHEME;
typedef HANDLE HPAINTBUFFER;

namespace Fog {

//! @addtogroup Fog_G2d_OS
//! @{

// ============================================================================
// [Fog::WinUxTheme]
// ============================================================================

//! @internal
//!
//! MSDN: Visual Styles Reference:
//!   http://msdn.microsoft.com/en-us/library/bb773178%28v=VS.85%29.aspx
struct FOG_API WinUxTheme
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  WinUxTheme();
  ~WinUxTheme();

  // --------------------------------------------------------------------------
  // [Initialization]
  // --------------------------------------------------------------------------

  err_t init();
  void close();

  // --------------------------------------------------------------------------
  // [API]
  // --------------------------------------------------------------------------

  // WinXP.
  enum { NUM_XP_SYMBOLS = 8 };
  union
  {
    struct
    {
      HTHEME  (FOG_STDCALL* pOpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
      HRESULT (FOG_STDCALL* pCloseThemeData)(HTHEME hTheme);
      HRESULT (FOG_STDCALL* pEnableTheming)(BOOL fEnable);

      BOOL    (FOG_STDCALL* pIsAppThemed)(void);

      HRESULT (FOG_STDCALL* pDrawThemeBackground)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, const RECT* pClipRect);
      HRESULT (FOG_STDCALL* pDrawThemeText)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect);

      HRESULT (FOG_STDCALL* pGetThemeBackgroundRegion)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCRECT pRect, HRGN *pRegion);
      BOOL    (FOG_STDCALL* pIsThemeBackgroundPartiallyTransparent)(HTHEME hTheme, int iPartId, int iStateId);
    };
    void* xp_addr[NUM_XP_SYMBOLS];
  };

  // WinVista.
  enum { NUM_VISTA_SYMBOLS = 4 };
  union
  {
    struct
    {
      HRESULT (FOG_STDCALL* pBufferedPaintInit)(void);
      HRESULT (FOG_STDCALL* pBufferedPaintUnInit)(void);
      HRESULT (FOG_STDCALL* pGetBufferedPaintBits)(HPAINTBUFFER hBufferedPaint, RGBQUAD **ppbBuffer, int *pcxRow);

      BOOL    (FOG_STDCALL* pIsCompositionActive)(void);
    };
    void* vista_addr[NUM_VISTA_SYMBOLS];
  };

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  Library dll;
  volatile err_t err;

private:
  err_t _init();

  FOG_NO_COPY(WinUxTheme)
};

//! @}

} // Fog namespace

// [Guard]
#endif // _FOG_G2D_OS_WINUXTHEME_H
