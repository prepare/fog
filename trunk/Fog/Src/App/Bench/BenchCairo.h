// [Fog-Bench]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_BENCHCAIRO_H
#define _FOG_BENCHCAIRO_H

// [Dependencies]
#include "BenchApp.h"

// [Dependencies - Cairo]
#include <cairo/cairo.h>

// ============================================================================
// [BenchFog]
// ============================================================================

struct BenchCairo : public BenchModule
{
  BenchCairo(BenchApp& app);
  virtual ~BenchCairo();

  // --------------------------------------------------------------------------
  // [Methods]
  // --------------------------------------------------------------------------

  virtual Fog::StringW getModuleName() const;
  virtual Fog::List<uint32_t> getSupportedPixelFormats() const;

  virtual void bench(BenchOutput& output, const BenchParams& params);

  virtual void prepareSprites(int size);
  virtual void freeSprites();

  // --------------------------------------------------------------------------
  // [Cairo-Specific]
  // --------------------------------------------------------------------------

  void configureContext(cairo_t* cr, const BenchParams& params);

  cairo_pattern_t* createLinearGradient(
    double x0, double y0,
    double x1, double y1,
    const Fog::Argb32& c0, const Fog::Argb32& c1, const Fog::Argb32& c2);

  void addRound(cairo_t* cr, const Fog::RectF& rect, float radius);

  // --------------------------------------------------------------------------
  // [Run]
  // --------------------------------------------------------------------------

  virtual void runCreateDestroy(BenchOutput& output, const BenchParams& params);
  virtual void runFillRectI(BenchOutput& output, const BenchParams& params);
  virtual void runFillRectF(BenchOutput& output, const BenchParams& params);
  virtual void runFillRectRotate(BenchOutput& output, const BenchParams& params);
  virtual void runFillRound(BenchOutput& output, const BenchParams& params);
  virtual void runFillPolygon(BenchOutput& output, const BenchParams& params, uint32_t complexity);
  virtual void runBlitImageI(BenchOutput& output, const BenchParams& params);
  virtual void runBlitImageF(BenchOutput& output, const BenchParams& params);
  virtual void runBlitImageRotate(BenchOutput& output, const BenchParams& params);

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  cairo_surface_t* screenCairo;
  Fog::List<cairo_surface_t*> spritesCairo;
};

// [Guard]
#endif // _FOG_BENCHCAIRO_H
