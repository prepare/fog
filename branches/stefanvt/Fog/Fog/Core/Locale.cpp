// [Fog-Core Library - Public API]
//
// [License]
// MIT, See COPYING file in package

// [Precompiled Headers]
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER

#if defined(FOG_PRECOMP)
#include FOG_PRECOMP
#endif // FOG_PRECOMP

// [Dependencies]
#include <Fog/Core/Assert.h>
#include <Fog/Core/ByteArray.h>
#include <Fog/Core/Constants.h>
#include <Fog/Core/Locale.h>
#include <Fog/Core/String.h>
#include <Fog/Core/StringUtil.h>
#include <Fog/Core/TextCodec.h>

#include <wchar.h>
#include <locale.h>

#if defined(FOG_HAVE_XLOCALE_H) && defined(FOG_HAVE_XLOCALE_H_LOCALECONV_L)
#include <xlocale.h>
#endif

namespace Fog {

// ============================================================================
// [Fog::Locale]
// ============================================================================

Static<Locale::Data> Locale::sharedNull;
Static<Locale::Data> Locale::sharedPosix;
Static<Locale::Data> Locale::sharedUser;

Locale* Locale::sharedNullLocale;
Locale* Locale::sharedPosixLocale;
Locale* Locale::sharedUserLocale;

static Static<Locale> sharedNullL;
static Static<Locale> sharedPosixL;
static Static<Locale> sharedUserL;

static void _setDefaults(Locale::Data* d)
{
  d->data[LOCALE_CHAR_DECIMAL_POINT] = '.';
  d->data[LOCALE_CHAR_THOUSANDS_GROUP] = '\0';
  d->data[LOCALE_CHAR_ZERO] = '0';
  d->data[LOCALE_CHAR_PLUS] = '+';
  d->data[LOCALE_CHAR_MINUS] = '-';
  d->data[LOCALE_CHAR_SPACE] = ' ';
  d->data[LOCALE_CHAR_EXPONENTIAL] = 'e';
  d->data[LOCALE_CHAR_FIRST_THOUSANDS_GROUP] = 3;
  d->data[LOCALE_CHAR_NEXT_THOUSANDS_GROUP] = 3;
  d->data[LOCALE_CHAR_RESERVED] = 0;
}

static void _setLConv(Locale::Data* d, const struct lconv* conv)
{
  _setDefaults(d);

  if (conv)
  {
    wchar_t buf[4];

    if (mbstowcs(buf, conv->decimal_point, 3) == 1) d->data[LOCALE_CHAR_DECIMAL_POINT  ] = (uint32_t)buf[0];
    if (mbstowcs(buf, conv->thousands_sep, 3) == 1) d->data[LOCALE_CHAR_THOUSANDS_GROUP] = (uint32_t)buf[0];
    if (mbstowcs(buf, conv->positive_sign, 3) == 1) d->data[LOCALE_CHAR_PLUS           ] = (uint32_t)buf[0];
    if (mbstowcs(buf, conv->negative_sign, 3) == 1) d->data[LOCALE_CHAR_MINUS          ] = (uint32_t)buf[0];
  }
}

Locale::Locale() :
  _d(sharedNull->ref())
{
}

Locale::Locale(const Locale& other) :
  _d(other._d->ref())
{
}

Locale::Locale(Data* d) :
  _d(d)
{
}

Locale::Locale(const String& name) :
  _d(sharedNull->ref())
{
  set(name);
}

Locale::~Locale()
{
  _d->deref();
}

err_t Locale::_detach()
{
  if (isDetached()) return ERR_OK;

  Data* newd = new(std::nothrow) Data(*_d);
  if (!newd) return ERR_RT_OUT_OF_MEMORY;

  atomicPtrXchg(&_d, newd)->deref();
  return ERR_OK;
}

void Locale::free()
{
  atomicPtrXchg(&_d, sharedNull->ref())->deref();
}

// [Set]

bool Locale::set(const String& name)
{
  char lcNameA[512];
  if (!StringUtil::unicodeToLatin1(lcNameA, name.getData(), name.getLength())) return false;

  TemporaryByteArray<128> savedLocale(Str8(setlocale(LC_ALL, NULL)));

  if (setlocale(LC_ALL, lcNameA))
  {
    detach();
    _d->name = name;
    _setLConv(_d, localeconv());
  }
  else 
  {
    free();
  }

  setlocale(LC_ALL, savedLocale.getData());
  return !isNull();
}

bool Locale::set(const Locale& other)
{
  atomicPtrXchg(&_d, other._d->ref())->deref();
  return true;
}

err_t Locale::setValue(int id, uint32_t value)
{
  if ((uint)id >= (uint)LOCALE_CHAR_INVALID) return ERR_RT_INVALID_ARGUMENT;
  if (_d->data[id] == value) return ERR_OK;

  FOG_RETURN_ON_ERROR(detach());

  _d->data[id] = value;
  return ERR_OK;
}

// ============================================================================
// [Fog::Locale::Data]
// ============================================================================

Locale::Data::Data()
{
  refCount.init(1);
  memset(data, 0, sizeof(uint32_t) * LOCALE_CHAR_INVALID);
}

Locale::Data::Data(const Data& other) :
  name(other.name)
{
  refCount.init(1);
  memcpy(data, other.data, sizeof(uint32_t) * LOCALE_CHAR_INVALID);
}

Locale::Data::~Data()
{
}

void Locale::Data::deref()
{
  if (refCount.deref()) delete this;
}

} // Fog namespace

// ============================================================================
// [Library Initializers]
// ============================================================================

FOG_INIT_DECLARE err_t fog_locale_init(void)
{
  using namespace Fog;

  Locale::sharedNull.init();
  Locale::sharedNull->name = Ascii8("NULL");
  _setDefaults(Locale::sharedNull.instancep());

  Locale::sharedPosix.init();
  Locale::sharedPosix->name= Ascii8("POSIX");
  _setDefaults(Locale::sharedPosix.instancep());

  Locale::sharedUser.init();
  _setLConv(Locale::sharedUser.instancep(), localeconv());

  sharedNullL.initCustom1(Locale::sharedNull.instancep());
  sharedPosixL.initCustom1(Locale::sharedPosix.instancep());
  sharedUserL.initCustom1(Locale::sharedUser.instancep());

  Locale::sharedNullLocale = sharedNullL.instancep();
  Locale::sharedPosixLocale = sharedPosixL.instancep();
  Locale::sharedUserLocale = sharedUserL.instancep();

  return ERR_OK;
}

FOG_INIT_DECLARE void fog_locale_shutdown(void)
{
  using namespace Fog;

  Locale::sharedNull.destroy();
  Locale::sharedPosix.destroy();
  Locale::sharedUser.destroy();
}
