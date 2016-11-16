#include <Fog/Core.h>
#include <Fog/Graphics.h>
#include <Fog/Gui.h>
#include <Fog/Svg.h>
#include <Fog/Xml.h>

#include "Fog/Gui/Frame.h"

// This is for MY testing:)

using namespace Fog;

struct MyPopUp : public Widget
{
  MyPopUp() : Widget(WINDOW_INLINE_POPUP)
  {
  }

  virtual void onPaint(PaintEvent* e)
  {
    Painter* p = e->getPainter();
    p->setOperator(OPERATOR_SRC_OVER);

    // Clear everything to white.
    p->setSource(Argb(0xAAFFFFFF));
    p->fillAll();

    p->setSource(Argb(0xFF000000));
    p->drawText(IntPoint(0,0),Ascii8("TEXT TEXT"),getFont());
  }
};

#if 0
//TABP_TABITEM
//BP_PUSHBUTTON

struct XPButton : public ButtonBase {
  XPButton() : _hBitmap(0), _hdc(0), _default(false), TYPE(TABP_TABITEM), _rgn(0), isbutton(true) {
    _theme.openTheme(L"Button");
  }

  void setDefault(bool def) {
    _default = def;
  }

  virtual void onGeometry(GeometryEvent* e) {
    int width = _geometry.getWidth();
    int height = _geometry.getHeight();
  }

  FOG_INLINE int calcState() const {
    int state = 0;

    if(isbutton) {
      state = isMouseOver() ? PBS_HOT : (_default ? PBS_DEFAULTED : PBS_NORMAL);
      state = isMouseDown() ? PBS_PRESSED : state;
      state = !isEnabled() ? PBS_DISABLED : state;
    } else {
      state = isMouseOver() ? TIS_HOT : TIS_NORMAL;
      state = isMouseDown() ? TIS_SELECTED : state;
      state = !isEnabled() ? TIS_DISABLED : state;
    }

    return state;
  }

  void createBitmap(int width, int height) {
    int targetBPP = 32;
    // Define bitmap attributes.
    BITMAPINFO bmi;

    if(_hdc)
      DeleteDC(_hdc);

    if(_hBitmap)
      DeleteObject(_hBitmap);

    _hdc = CreateCompatibleDC(NULL);
    if (_hdc == NULL) {
      return;
    }

    //Fog::Memory::zero();
    Fog::Memory::zero(&bmi, sizeof(bmi));
    bmi.bmiHeader.biSize        = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth       = width;
    bmi.bmiHeader.biHeight      = -height;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = targetBPP;
    bmi.bmiHeader.biCompression = BI_RGB;

    unsigned char* pixels = NULL;

    _hBitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
    if (_hBitmap == NULL) 
    {
      DeleteDC(_hdc);
      return;
    }

    // Select bitmap to DC.
    SelectObject(_hdc, _hBitmap);

    DIBSECTION info;
    GetObjectW(_hBitmap, sizeof(DIBSECTION), &info);

    _buffer.data = pixels;
    _buffer.width = width;
    _buffer.height = height;
    _buffer.stride = info.dsBm.bmWidthBytes;

    if(_theme.IsThemeBackgroundPartiallyTransparent(TYPE,calcState())) {
      _buffer.format = IMAGE_FORMAT_PRGB32;
    } else {
      _buffer.format = IMAGE_FORMAT_XRGB32;
    }

    _image.adopt(_buffer);
  }

  virtual void onPaint(PaintEvent* e) {    
    IntRect r = getClientContentGeometry(); //for easy margin support
    RECT rect;
    rect.left = r.x;
    rect.right = r.getWidth();
    rect.top = r.y;
    rect.bottom = r.getHeight();

    //createBitmap(r.getWidth(), r.getHeight());

    int state = calcState();

    if(_theme.IsThemeBackgroundPartiallyTransparent(TYPE,state)) {
      //button
      //_image.clear(Argb(0x00000000));
      //_theme.getThemeBackgroundRegion(_hdc,TYPE,state,&rect,&_rgn);
      //SelectClipRgn(_hdc, _rgn);
    } else {
      //tab
      _image.clear(Argb(0xFFFFFFFF));
    }    

    //Clip on Region to support Transparency! ... needed?

    BOOL result = _theme.drawThemeBackground(_hdc, TYPE,  state, &rect, NULL);

    //draws with a really ugly font... why?? (also if I set the font within window -> maybe because I open them with null hwnd)
    //BOOL ret = _theme.drawThemeText(_hdc,BP_PUSHBUTTON,state,(wchar_t*)_text.getData(),_text.getLength(),DT_CENTER|DT_VCENTER|DT_SINGLELINE,0,&rect);     
    e->getPainter()->setSource(Argb(0xFFFFFFFF));
    e->getPainter()->fillRect(IntRect(0, 0, getWidth(), getHeight()));
    e->getPainter()->drawImage(IntPoint(0,0),_image);
    e->getPainter()->setSource(Argb(0xFF000000));
    e->getPainter()->drawText(r, _text, _font, TEXT_ALIGN_CENTER);
  }

private:
  UxTheme _theme;
  HBITMAP _hBitmap;
  HRGN _rgn;
  HDC _hdc;

  ImageBuffer _buffer;

  Image _image;
  bool _default;

  const int TYPE;

  const int isbutton;
};
#endif

struct MyModalWindow : public Window
{
  MyModalWindow(int x) : Window(WINDOW_TYPE_DEFAULT), _x(x)
  {
    Button* button5 = new Button();
    addChild(button5);
    button5->setGeometry(IntRect(40, 200, 100, 20));
    button5->setText(Ascii8("Test ModalWindow"));
    button5->show();  
    button5->addListener(EVENT_CLICK, this, &MyModalWindow::onModalTestClick);
  }

  void onModalTestClick(MouseEvent* e)
  {
    _modalwindow = new MyModalWindow(_x+1);
    _modalwindow->setSize(IntSize(200,300));
    _modalwindow->addListener(EVENT_CLOSE, this, &MyModalWindow::onModalClose);
    _modalwindow->showModal(getGuiWindow());
  }

  void onModalClose()
  {
    _modalwindow->hide();
    //_modalwindow->destroy();
    _modalwindow = 0;
  }

  virtual void onPaint(PaintEvent* e)
  {
    Painter* p = e->getPainter();
    p->setOperator(OPERATOR_SRC);
    // Clear everything to white.
    p->setSource(Argb(0xFF000000));
    p->fillAll();

    p->setSource(Argb(0xFFFFFFFF));
    String s;
    s.format("MODAL DIALOG NO: %i", _x);
    p->drawText(IntPoint(0,0),s,getFont());
  }

  int _x;
  MyModalWindow* _modalwindow;
};

struct MyWindow : public Window
{
  // [Fog Object System]
  FOG_DECLARE_OBJECT(MyWindow, Window)

  // [Construction / Destruction]
  MyWindow(uint32_t createFlags = 0);
  virtual ~MyWindow();

  // [Event Handlers]
  virtual void onPaint(PaintEvent* e);

  void createButtons(int count)
  {
    _buttons.clear();
    _buttons.reserve(count);

    for(int i=0;i<count;++i) {
      Button* buttonx1 = new Button();
      addChild(buttonx1);
      //button4->setGeometry(IntRect(40, 160, 100, 20));
      String str;
      str.format("XButton%i", i);
      buttonx1->setText(str);
      buttonx1->show(); 

      // buttonx1->setMinimumSize(IntSize(40,40));
      _buttons.append(buttonx1);
    }

  }

  void testBorderLayout(Layout* parent=0)
  {
    createButtons(6);
    BorderLayout* layout;
    if(parent) {
      layout = new BorderLayout();      
    } else {
      layout = new BorderLayout(this);
    }

    layout->addItem(_buttons.at(0), LAYOUT_EDGE_SOUTH);
    layout->addItem(_buttons.at(1), LAYOUT_EDGE_WEST);
    layout->addItem(_buttons.at(2), LAYOUT_EDGE_CENTER);
    layout->addItem(_buttons.at(3), LAYOUT_EDGE_EAST);
    layout->addItem(_buttons.at(4), LAYOUT_EDGE_NORTH);
    layout->addItem(_buttons.at(5), LAYOUT_EDGE_NORTH);

    LayoutItem* item = _buttons.at(3);
    BorderLayout::LayoutData* d = (BorderLayout::LayoutData*)item->_layoutdata;
    item->getLayoutProperties<BorderLayout>()->setFlex(1);
    d = d;
  }

  void testGridLayout(Layout* parent=0)
  {
    createButtons(8);

    GridLayout* layout;
    if(parent) {
      layout = new GridLayout();      
    } else {
      layout = new GridLayout(this);
    }

    layout->addItem(_buttons.at(0),0,0);
    layout->addItem(_buttons.at(1),0,1);
    layout->addItem(_buttons.at(2),0,2);
    layout->addItem(_buttons.at(3),1,0,1,3);
    layout->addItem(_buttons.at(4),2,0,2);
    layout->addItem(_buttons.at(5),2,1);
    layout->addItem(_buttons.at(6),3,1);
    layout->addItem(_buttons.at(6),3,2);

    layout->setRowFlex(2, 1);
    layout->setColumnFlex(1, 1);    
    
  }

  void onButtonClick(MouseEvent* ev)
  {
    _buttons.at(0)->getLayoutProperties<VBoxLayout>()->setFlex(1);
    _layout->_dirty = 1;
    invalidateLayout();
  }

  void testVBoxLayout(Layout* parent = 0)
  {
    const int COUNT = 8;
    createButtons(COUNT);

    VBoxLayout* layout;
    if(parent) {
      layout = new VBoxLayout(0,0);      
    } else {
      layout = new VBoxLayout(this,0,0);
    }

    for(int i=0;i<COUNT;++i) {
      layout->addItem(_buttons.at(i));
    }

    layout->setSpacing(2);

    _buttons.at(3)->getLayoutProperties<VBoxLayout>()->setFlex(1);
    _buttons.at(2)->getLayoutProperties<VBoxLayout>()->setFlex(2);

    _buttons.at(0)->addListener(EVENT_CLICK,this,&MyWindow::onButtonClick);

    CheckBox* cb = new CheckBox();
    cb->setText(Ascii8("Check box"));
    cb->show();
    layout->addItem(cb);
  }

  void testHBoxLayout(Layout* parent = 0)
  {
    const int COUNT = 8;
    createButtons(COUNT);
    HBoxLayout* layout;
    if(parent) {
      layout = new HBoxLayout(0,0);      
    } else {
      layout = new HBoxLayout(this,0,0);
    }

    for(int i=0;i<COUNT;++i) {
      layout->addItem(_buttons.at(i));
    }

    _buttons.at(3)->getLayoutProperties<HBoxLayout>()->setFlex(1);
    _buttons.at(2)->getLayoutProperties<HBoxLayout>()->setFlex(2);
  }

  void testFrame()
  {
    VBoxLayout* layout = new VBoxLayout(this, 0, 0);

    {
      TextField* textField = new TextField();
      textField->show();
      textField->setMinimumSize(IntSize(100, 20));
      addChild(textField);
      layout->addItem(textField);
    }

    {
      TextField* textField = new TextField();
      textField->show();
      textField->setMinimumSize(IntSize(100, 20));
      addChild(textField);
      layout->addItem(textField);
    }
  }

  void testNestedLayout()
  {
  }

  void onModalClose()
  {
    _modalwindow->hide();
    _modalwindow->destroy();
    _modalwindow = 0;
  }

  virtual void onClose(CloseEvent* e)
  {
    this->destroyWindow();
  }

  void onModalTestClick(MouseEvent* e)
  {
    _modalwindow = new MyModalWindow(_mcount);
    _modalwindow->setSize(IntSize(200,300));
    _modalwindow->addListener(EVENT_CLOSE, this, &MyWindow::onModalClose);
    _modalwindow->showModal(getGuiWindow());
  }

  void onPopUpClick(MouseEvent* e)
  {
    if(_popup->getVisibility() < WIDGET_VISIBLE) {
      _popup->show();
    }
  }

  void onFrameTestClick(MouseEvent* e)
  {
    if(getWindowFlags() & WINDOW_NATIVE) {
      setWindowFlags(WINDOW_TYPE_TOOL|WINDOW_TRANSPARENT);
    } else {
      setWindowFlags(WINDOW_TYPE_DEFAULT|WINDOW_TRANSPARENT);
    }
  }

  void onFullScreenClick(MouseEvent* e)
  {
    if(isFullScreen()) {
      show();
    } else {      
      show(WIDGET_VISIBLE_FULLSCREEN);
    }
  }

  void onTransparencyClick(MouseEvent* e)
  {
    WidgetOpacityAnimation* anim = new WidgetOpacityAnimation(this);
    anim->setDuration(TimeDelta::fromMilliseconds(1000));

    anim->setStartOpacity(0.0f);
    anim->setEndOpacity(1.0f);
    
    if(getTransparency() == 1.0) {
      anim->setDirection(ANIMATION_BACKWARD);
      anim->setStartOpacity(0.1f);
    }

    Application::getInstance()->getAnimationDispatcher()->addAnimation(anim);
  }

  void paintImage(Painter* painter, const IntPoint& pos, const Image& im, const String& name);

  Button* button;

  Image i[2];
  double _subx;
  double _suby;
  double _rotate;
  double _shearX;
  double _shearY;
  double _scale;
  int _spread;

  Widget* _popup;
  int _mcount;
  MyModalWindow* _modalwindow;


  List<Widget*> _buttons;

  //FlowLayout* _layout;
};

FOG_IMPLEMENT_OBJECT(MyWindow)

MyWindow::MyWindow(uint32_t createFlags) :
  Window(createFlags)
{
  setWindowTitle(Ascii8("Testing..."));

  i[0].readFromFile(Ascii8("babelfish.png"));
  i[1].readFromFile(Ascii8("kweather.png"));

  i[0].convert(IMAGE_FORMAT_PRGB32);
  i[1].convert(IMAGE_FORMAT_PRGB32);

  _mcount = 0;

  _subx = 0.0;
  _suby = 0.0;
  _rotate = 0.0;
  _shearX = 0.0;
  _shearY = 0.0;
  _scale = 1.0;
  _spread = PATTERN_SPREAD_REPEAT;

  //testVBoxLayout();
  testFrame();
  //setContentRightMargin(0);
}

MyWindow::~MyWindow()
{
}

void MyWindow::onPaint(PaintEvent* e)
{
  TimeTicks ticks = TimeTicks::highResNow();
  Painter* p = e->getPainter();

  p->save();
  p->setOperator(OPERATOR_SRC);

  // Clear everything to white.
  p->setSource(Argb(0xFFFFFFFF));
  p->fillAll();

  p->restore();
}

// ============================================================================
// [MAIN]
// ============================================================================

FOG_GUI_MAIN()
{
  Application app(Ascii8("Gui"));

  MyWindow window(WINDOW_TYPE_DEFAULT);
  window.setSize(IntSize(100, 100));
  window.show();

  app.addListener(EVENT_LAST_WINDOW_CLOSED, &app, &Application::quit);
  return app.run();
}
