// [Fog-Gui Library - Public API]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_GUI_LAYOUT_LAYOUTITEM_H
#define _FOG_GUI_LAYOUT_LAYOUTITEM_H

// [Dependencies]
#include <Fog/Core/Constants.h>
#include <Fog/Core/Object.h>
#include <Fog/Graphics/Geometry.h>
#include <Fog/Gui/Constants.h>
#include <Fog/Gui/Layout/LayoutPolicy.h>
#include <Fog/Gui/Layout/LayoutHint.h>

namespace Fog {

//! @addtogroup Fog_Gui_Layout
//! @{

// ============================================================================
// [Forward Declarations]
// ============================================================================

struct Layout;
struct Widget;
struct SpacerItem;

// ============================================================================
// [Fog::LayoutItem]
// ============================================================================

//! @brief Base for @c Widget and @c Layout classes.
struct FOG_API LayoutItem : public Object
{
  FOG_DECLARE_OBJECT(LayoutItem, Object)

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  LayoutItem(uint32_t alignment = 0);
  virtual ~LayoutItem();

  // --------------------------------------------------------------------------
  // [ContentMargins getter]
  // --------------------------------------------------------------------------

  FOG_INLINE IntMargins getContentMargins() const { return _contentmargin; }
  FOG_INLINE int getContentLeftMargin() const { return _contentmargin.left; }
  FOG_INLINE int getContentRightMargin() const { return _contentmargin.right; }
  FOG_INLINE int getContentTopMargin() const { return _contentmargin.top; }
  FOG_INLINE int getContentBottomMargin() const { return _contentmargin.bottom; }

  FOG_INLINE int getContentXMargins() const { return _contentmargin.left + _contentmargin.right; }
  FOG_INLINE int getContentYMargins() const { return _contentmargin.top + _contentmargin.bottom; }

  // --------------------------------------------------------------------------
  // [ContentMargins setter]
  // --------------------------------------------------------------------------

  enum MarginPosition {MARGIN_LEFT, MARGIN_RIGHT, MARGIN_TOP, MARGIN_BOTTOM};

  //!@brief this method allows to write a margin filter
  FOG_INLINE virtual int calcMargin(int margin, MarginPosition pos) const { return margin; }

  //TODO: only invalidate if margin really changes after calcMargin
  FOG_INLINE void checkMarginChanged(int a, int b) { if (a != b) { _dirty = 1; updateLayout();} }
  FOG_INLINE void setContentMargins(const IntMargins& m)  { setContentMargins(m.left,m.right,m.top,m.bottom); }
  FOG_INLINE void setContentMargins(int left, int right, int top, int bottom)  { _contentmargin.set(calcMargin(left,MARGIN_LEFT),calcMargin(right,MARGIN_RIGHT),calcMargin(top,MARGIN_TOP),calcMargin(bottom,MARGIN_BOTTOM)); checkMarginChanged(1,2); }  //TODO: optimize update!
  FOG_INLINE void setContentLeftMargin(int m)  { int z=_contentmargin.left; _contentmargin.left = calcMargin(m,MARGIN_LEFT); checkMarginChanged(z,_contentmargin.left); }
  FOG_INLINE void setContentRightMargin(int m)  { int z=_contentmargin.right; _contentmargin.right = calcMargin(m,MARGIN_RIGHT); checkMarginChanged(z,_contentmargin.right);}
  FOG_INLINE void setContentTopMargin(int m)  { int z=_contentmargin.top; _contentmargin.top = calcMargin(m,MARGIN_TOP); checkMarginChanged(z,_contentmargin.top); }
  FOG_INLINE void setContentBottomMargin(int m)  { int z=_contentmargin.bottom; _contentmargin.bottom = calcMargin(m,MARGIN_BOTTOM); checkMarginChanged(z,_contentmargin.bottom); }

  // --------------------------------------------------------------------------
  // [Height For Width]
  // --------------------------------------------------------------------------

  virtual bool hasLayoutHeightForWidth() const { return false; }
  virtual int getLayoutHeightForWidth(int width) const { return -1; }
  FOG_INLINE virtual int getLayoutMinimumHeightForWidth(int width) const { return getLayoutHeightForWidth(width); }

  // --------------------------------------------------------------------------
  // [LayoutHint]
  // --------------------------------------------------------------------------

  //! @brief Mark this layout for a need to repaint
  FOG_INLINE virtual void updateLayout();

  virtual void calculateLayoutHint(LayoutHint& hint) = 0;

  //TODO: maybe we can insert clearDirty() in special function, so that it is only
  //checked there, to have a real const getter!

  FOG_INLINE void clearDirty() {
    FOG_ASSERT(_dirty);
    if (!isEmpty()) {
      calculateLayoutHint(_cache);
      _propertydirty = 0;
    }
    else
    {
      _cache._maximumSize = _cache._minimumSize = _cache._sizeHint = IntSize(0,0);
    }
   
    _dirty = 0;    
  }

  FOG_INLINE const LayoutHint& getLayoutHint() const {
    if (_dirty) {
      const_cast<LayoutItem*>(this)->clearDirty();
    }

    if (isLayout())
    {
      int i = 100;
    }

    return _cache;
  }

  FOG_INLINE const IntSize& getLayoutSizeHint() const { return getLayoutHint()._sizeHint; }
  FOG_INLINE const IntSize& getLayoutMinimumSize() const { return getLayoutHint()._minimumSize; }
  FOG_INLINE const IntSize& getLayoutMaximumSize() const { return getLayoutHint()._maximumSize; }

  // --------------------------------------------------------------------------
  // [Calculate SizeHint]
  // --------------------------------------------------------------------------

  IntSize calculateMinimumSize() const;
  IntSize calculateMaximumSize() const;

  // --------------------------------------------------------------------------
  // [Expanding Directions]
  // --------------------------------------------------------------------------

  virtual uint32_t getLayoutExpandingDirections() const = 0;

  // --------------------------------------------------------------------------
  // [Visibility]
  // --------------------------------------------------------------------------

  virtual bool isEmpty() const = 0;  

  // --------------------------------------------------------------------------
  // [Cache Handling]
  // --------------------------------------------------------------------------

  //virtual void invalidateLayout() { _dirty = 1; }

  // --------------------------------------------------------------------------
  // [Geometry]
  // --------------------------------------------------------------------------

  virtual void setLayoutGeometry(const IntRect&) = 0;
  virtual IntRect getLayoutGeometry() const { return IntRect(); }

  // --------------------------------------------------------------------------
  // [Alignment]
  // --------------------------------------------------------------------------

  uint32_t getLayoutAlignment() const { return _alignment; }
  void setLayoutAlignment(uint32_t a) { _alignment = a; }

  // --------------------------------------------------------------------------
  // [Flex variable support]
  // --------------------------------------------------------------------------
  
  void removeLayoutStruct();

  // --------------------------------------------------------------------------
  // [Members]
  // -------------------------------------------------------------------------- 

  template<class T> typename T::PropertyType* getLayoutProperties() {
    if (!_layoutdata) return 0;
    typename T::LayoutData* prop = ((typename T::LayoutData*)_layoutdata);
    return (T::PropertyType*) &((typename T::LayoutData*)_layoutdata)->_user;
  }

  template<class T>
  T* getLayoutData() {
    return static_cast<T*>(_layoutdata);
  }

  //Struct for Calculation of Flex-Values in LayoutManager
  struct LayoutData
  {
  };

  struct FlexLayoutData : public LayoutData {
    FlexLayoutData() : _min(0), _max(0), _hint(0), _flex(-1), _potential(0), _offset(0), _next(0) {}
    int _min;     //height or width
    int _max;     //height or width
    int _hint;    //height or width

    // Request/Response
    float _flex;  //the flex value for this Item

    int _potential; //for internal use
    int _offset;  //for internal use

    void* _next;   //for faster Flexible handling (could be different type)

    FOG_INLINE void init(bool grow)
    {
      _offset = 0;
      _potential = grow ?_max - _hint : _hint - _min;
      _flex = grow ? _flex : (1 / _flex);
    }

    FOG_INLINE void prepare() { _potential = -INT_MAX; }
  };  

  LayoutData* _layoutdata;

  IntMargins _contentmargin;
  LayoutHint _cache;
  //! @brief For fast identification of Layout, where this Item is inserted!
  //! maybe also used for layout pointer in widget
  Layout* _withinLayout; 
  
  uint32_t _alignment : 2;
  //! @brief Layout hint is dirty -> call calculateLayoutHint() next time
  uint32_t _dirty : 1;
  //! @brief Property values changes -> calculate.
  uint32_t _propertydirty : 1;
  uint32_t _unused : 28;
};

// TODO: invalidateLayout.
#define FOG_LAYOUT_PROPERTY_CHANGED \
  _layout->_propertydirty = 1;

#define FOG_DECLARE_FLEX_PROPERTY() \
public:\
  FOG_INLINE bool hasFlex() const { return _flex > 0; } \
  FOG_INLINE int getFlex() const { return _flex; } \
  FOG_INLINE void setFlex(int value) { if (value == _flex) return; _flex = value; FOG_LAYOUT_PROPERTY_CHANGED} \
  FOG_INLINE void clear() { _flex = -1; } \
private: \
  int _flex;

#define FOG_INIT_FLEX_PROPERTY() \
  _flex = -1;

#define FOG_DECLARE_PERCENT_HEIGHT_PROPERTY() \
public: \
  FOG_INLINE bool hasHeight() const { return _height != -1.f; } \
  FOG_INLINE void setHeight(float value) { if (value == _height) return; _height = value > 1.f ? 1.f : value < 0.f ? 0.f : value; FOG_LAYOUT_PROPERTY_CHANGED } \
  FOG_INLINE float getHeight() const { return _height; } \
  FOG_INLINE void clearHeight() { if (_height == -1.f) return; _height = -1.f; FOG_LAYOUT_PROPERTY_CHANGED } \
private: \
  float _height;

#define FOG_INIT_PERCENT_HEIGHT_PROPERTY() \
  _height = 1.f;

#define FOG_DECLARE_PERCENT_WIDTH_PROPERTY()\
public: \
  FOG_INLINE bool hasWidth() const { return _width != -1.f; } \
  FOG_INLINE void setWidth(float value) {if (value == _width) return; _width = value > 1.f ? 1.f : value < 0.f ? 0.f : value; FOG_LAYOUT_PROPERTY_CHANGED } \
  FOG_INLINE float getWidth() const { return _width; } \
  FOG_INLINE void clearWidth() {if (_width == -1.f) return; _width = -1.f; FOG_LAYOUT_PROPERTY_CHANGED } \
private: \
  float _width;

#define FOG_INIT_PERCENT_WIDTH_PROPERTY() \
  _width=1.f;
       
#define FOG_DECLARE_PERCENT_SIZE_PROPERTY() \
  FOG_DECLARE_PERCENT_HEIGHT_PROPERTY() \
  FOG_DECLARE_PERCENT_WIDTH_PROPERTY()

#define FOG_INIT_PERCENT_SIZE_PROPERTY() \
  FOG_INIT_PERCENT_HEIGHT_PROPERTY() \
  FOG_INIT_PERCENT_WIDTH_PROPERTY()

} // Fog namespace

//! @}

// [Guard]
#endif // _FOG_GUI_LAYOUT_LAYOUTITEM_H
