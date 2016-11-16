#include <Fog/Core.h>
#include <Fog/G2d.h>
#include <Fog/UI.h>

#include "Resources.h"

using namespace Fog;

// ============================================================================
// [AppWindow - Declaration]
// ============================================================================

struct AppWindow : public UIEngineWindow
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  AppWindow(UIEngine* engine, uint32_t hints = 0);
  virtual ~AppWindow();

  // --------------------------------------------------------------------------
  // [Event Handlers]
  // --------------------------------------------------------------------------

  virtual void onEngineEvent(UIEngineEvent* ev);
  virtual void onPaint(Painter* p);

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  Image background;
  Image sprite[2];
};

// ============================================================================
// [SampleWindow - Construction / Destruction]
// ============================================================================

AppWindow::AppWindow(UIEngine* engine, uint32_t hints) :
  UIEngineWindow(engine, hints)
{
  background.create(SizeI(40, 40), IMAGE_FORMAT_XRGB32);
  background.fillRect(RectI( 0,  0, 20, 20), Argb32(0xFFFFFFFF));
  background.fillRect(RectI(20,  0, 20, 20), Argb32(0xFFCFCFCF));
  background.fillRect(RectI( 0, 20, 20, 20), Argb32(0xFFCFCFCF));
  background.fillRect(RectI(20, 20, 20, 20), Argb32(0xFFFFFFFF));

  sprite[0].readFromBuffer(_resource_babelfish_png, FOG_ARRAY_SIZE(_resource_babelfish_png));
  sprite[1].readFromBuffer(_resource_firewall_png, FOG_ARRAY_SIZE(_resource_firewall_png));
}

AppWindow::~AppWindow()
{
}

// ============================================================================
// [SampleWindow - Event Handlers]
// ============================================================================

void AppWindow::onEngineEvent(UIEngineEvent* ev)
{
  switch (ev->getCode())
  {
    case UI_ENGINE_EVENT_CLOSE:
      Application::get()->quit();
      break;

    case UI_ENGINE_EVENT_PAINT:
      onPaint(static_cast<UIEnginePaintEvent*>(ev)->getPainter());
      break;
  }
}

void AppWindow::onPaint(Painter* _p)
{
  Painter& p = *_p;

  p.setSource(Argb32(0xFFFFFFFF));
  p.fillAll();

  SizeI s = sprite[0].getSize();
  int x = 0;
  int y = 0;

  for (uint32_t op = 0; op < COMPOSITE_COUNT; op++)
  {
    PointI pos(x * (s.w + 10) + 10, y * (s.h + 10) + 10);

    p.setSource(Texture(background, TEXTURE_TILE_REPEAT));
    p.fillRect(RectI(pos.x, pos.y, 128, 128));

    p.setSource(Argb32(0xFF000000));
    p.drawRect(RectF(float(pos.x) - 0.5f, float(pos.y) - 0.5f, 129.0f, 129.0f));

    p.beginGroup();
    p.setCompositingOperator(COMPOSITE_SRC);
    p.blitImage(pos, sprite[0]);
    p.setCompositingOperator(op);
    p.blitImage(pos, sprite[1]);
    p.paintGroup();

    if (++x >= 5) { y++; x = 0; }
  }
}

// ============================================================================
// [FOG_UI_MAIN]
// ============================================================================

FOG_UI_MAIN()
{
  Application app(StringW::fromAscii8("UI"));
  AppWindow wnd(app.getUIEngine());

  wnd.setWindowTitle(StringW::fromAscii8("FogBlend"));
  wnd.setWindowSize(SizeI(720, 740));
  wnd.show();

  return app.run();
}
