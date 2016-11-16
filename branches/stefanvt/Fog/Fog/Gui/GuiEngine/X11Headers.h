// [Fog-Gui Library - Public API]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_GUI_GUIENGINE_X11HEADERS_H
#define _FOG_GUI_GUIENGINE_X11HEADERS_H

// [Dependencies]
#include <Fog/Build/Build.h>

// [Guard]
#if defined(FOG_GUI_X11)

#ifdef _XLIB_H
#error "You must include Fog-Gui/UIEngine/X11Headers.h before xlib.h"
#endif

// redefine some critical things

#define Atom XAtom
#define Colormap XColormap
#define Complex XComplex
#define Display XDisplay
#define Picture XPicture
#define Region XRegion
#define Time XTime
#define Visual XVisual
#define Window XWindow

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#define XK_MISCELLANY
#define XK_LATIN1
#define XK_XKB_KEYS
#include <X11/keysymdef.h>
#include <X11/Xpoll.h>

// We need XShm extension to speedup our blits to top level windows.
#include <X11/extensions/XShm.h>
// We need XRender extension?
#include <X11/extensions/Xrender.h>
// We need shape extension to enable non-rectangular windows.
#include <X11/extensions/shape.h>

#undef Atom
typedef XAtom Atom;
#undef Colormap
#undef Complex
#undef Display
#undef Picture
#undef Region
#undef Time
#undef Visual
#undef Window

#ifdef Above
const int XAbove = Above;
#undef Above
#endif

#ifdef Always
const int XAlways = Always;
#undef Always
#endif

#ifdef Below
const int XBelow = Below;
#undef Below
#endif

#ifdef Bool
typedef Bool XBool;
#undef Bool
#endif

#ifdef ButtonPress
const int XButtonPress = ButtonPress;
#undef ButtonPress
#endif

#ifdef ButtonRelease
const int XButtonRelease = ButtonRelease;
#undef ButtonRelease
#endif

#ifdef CirculateNotify
const int XCirculateNotify = CirculateNotify;
#undef CirculateNotify
#endif

#ifdef CirculateRequest
const int XCirculateRequest = CirculateRequest;
#undef CirculateRequest
#endif

#ifdef ClientMessage
const int XClientMessage = ClientMessage;
#undef ClientMessage
#endif

#ifdef ColormapNotify
const int XColormapNotify = ColormapNotify;
#undef ColormapNotify
#endif

#ifdef ConfigureNotify
const int XConfigureNotify = ConfigureNotify;
#undef ConfigureNotify
#endif

#ifdef ConfigureRequest
const int XConfigureRequest = ConfigureRequest;
#undef ConfigureRequest
#endif

#ifdef CreateNotify
const int XCreateNotify = CreateNotify;
#undef CreateNotify
#endif

#ifdef CursorShape
const int XCursorShape = CursorShape;
#undef CursorShape
#endif

#ifdef DestroyNotify
const int XDestroyNotify = DestroyNotify;
#undef DestroyNotify
#endif

#ifdef EnterNotify
const int XEnterNotify = EnterNotify;
#undef EnterNotify
#endif

#ifdef Expose
const int XExpose = Expose;
#undef Expose
#endif

#ifdef FocusIn
const int XFocusIn = FocusIn;
#undef FocusIn
#endif

#ifdef FocusOut
const int XFocusOut = FocusOut;
#undef FocusOut
#endif

#ifdef GraphicsExpose
const int XGraphicsExpose = GraphicsExpose;
#undef GraphicsExpose
#endif

#ifdef GravityNotify
const int XGravityNotify = GravityNotify;
#undef GravityNotify
#endif

#ifdef GrayScale
const int XGrayScale = GrayScale;
#undef GrayScale
#endif

#ifdef LeaveNotify
const int XLeaveNotify = LeaveNotify;
#undef LeaveNotify
#endif

#ifdef KeymapNotify
const int XKeymapNotify = KeymapNotify;
#undef KeymapNotify
#endif

#ifdef KeyPress
const int XKeyPress = KeyPress;
#undef KeyPress
#endif

#ifdef KeyRelease
const int XKeyRelease = KeyRelease;
#undef KeyRelease
#endif

#ifdef MapNotify
const int XMapNotify = MapNotify;
#undef MapNotify
#endif

#ifdef MappingNotify
const int XMappingNotify = MappingNotify;
#undef MappingNotify
#endif

#ifdef MapRequest
const int XMapRequest = MapRequest;
#undef MapRequest
#endif

#ifdef MotionNotify
const int XMotionNotify = MotionNotify;
#undef MotionNotify
#endif

#ifdef NoExpose
const int XNoExpose = NoExpose;
#undef NoExpose
#endif

#ifdef NotifyAncestor
const int XNotifyAncestor = NotifyAncestor;
#undef NotifyAncestor
#endif

#ifdef NotifyGrab
const int XNotifyGrab = NotifyGrab;
#undef NotifyGrab
#endif

#ifdef NotifyInferior
const int XNotifyInferior = NotifyInferior;
#undef NotifyInferior
#endif

#ifdef NotifyNonlinear
const int XNotifyNonlinear = NotifyNonlinear;
#undef NotifyNonlinear
#endif

#ifdef NotifyNonlinearVirtual
const int XNotifyNonlinearVirtual = NotifyNonlinearVirtual;
#undef NotifyNonlinearVirtual
#endif

#ifdef NotifyNormal
const int XNotifyNormal = NotifyNormal;
#undef NotifyNormal
#endif

#ifdef NotifyUngrab
const int XNotifyUngrab = NotifyUngrab;
#undef NotifyUngrab
#endif

#ifdef NotifyVirtual
const int XNotifyVirtual = NotifyVirtual;
#undef NotifyVirtual
#endif

#ifdef None
const XID XNone = None;
#undef None
#endif

#ifdef PropertyNotify
const int XPropertyNotify = PropertyNotify;
#undef PropertyNotify
#endif

#ifdef ReparentNotify
const int XReparentNotify = ReparentNotify;
#undef ReparentNotify
#endif

#ifdef ResizeRequest
const int XResizeRequest = ResizeRequest;
#undef ResizeRequest
#endif

#ifdef SelectionClear
const int XSelectionClear = SelectionClear;
#undef SelectionCleart
#endif

#ifdef SelectionNotify
const int XSelectionNotify = SelectionNotify;
#undef SelectionNotify
#endif

#ifdef SelectionRequest
const int XSelectionRequest = SelectionRequest;
#undef SelectionRequest
#endif

#ifdef Status
typedef Status XStatus;
#undef Status
#endif

#ifdef Success
const int XSuccess = Success;
#undef Success
#endif

#ifdef UnmapNotify
const int XUnmapNotify = UnmapNotify;
#undef UnmapNotify
#endif

#ifdef Unsorted
const int XUnsorted = Unsorted;
#undef Unsorted
#endif

#ifdef VisibilityNotify
const int XVisibilityNotify = VisibilityNotify;
#undef VisibilityNotify
#endif

// index is used for methods...
#if defined(index)
#undef index
#endif // index

#else
#warning "Fog::X11 - Including Fog-Gui/UIEngine/X11Headers on non-x11 plaform"
#endif // FOG_GUI_X11

// [Guard]
#endif // _FOG_GUI_GUIENGINE_X11HEADERS_H
