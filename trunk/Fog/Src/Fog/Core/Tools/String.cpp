// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// [Precompiled Headers]
#if defined(FOG_PRECOMP)
#include FOG_PRECOMP
#endif // FOG_PRECOMP

// [Dependencies]
#include <Fog/Core/Global/Init_p.h>
#include <Fog/Core/Global/Private.h>
#include <Fog/Core/Memory/MemMgr.h>
#include <Fog/Core/Memory/BSwap.h>
#include <Fog/Core/Tools/Algorithm.h>
#include <Fog/Core/Tools/Char.h>
#include <Fog/Core/Tools/CharData.h>
#include <Fog/Core/Tools/CharUtil.h>
#include <Fog/Core/Tools/CharUtilPrivate_p.h>
#include <Fog/Core/Tools/ContainerUtil.h>
#include <Fog/Core/Tools/HashUtil.h>
#include <Fog/Core/Tools/List.h>
#include <Fog/Core/Tools/Locale.h>
#include <Fog/Core/Tools/RegExp.h>
#include <Fog/Core/Tools/String.h>
#include <Fog/Core/Tools/StringTmp_p.h>
#include <Fog/Core/Tools/StringUtil.h>
#include <Fog/Core/Tools/StringUtil_dtoa_p.h>
#include <Fog/Core/Tools/TextCodec.h>
#include <Fog/Core/Tools/VarId.h>

namespace Fog {

// ============================================================================
// [Fog::String - Global]
// ============================================================================

static Static<StringDataA> StringA_dEmpty;
static Static<StringDataW> StringW_dEmpty;

static Static<StringA> StringA_oEmpty;
static Static<StringW> StringW_oEmpty;

template<typename CharT>
FOG_STATIC_INLINE_T CharT_(StringData)* StringT_getDEmpty() { return NULL; }

template<> FOG_STATIC_INLINE_T StringDataA* StringT_getDEmpty<char>() { return &StringA_dEmpty; }
template<> FOG_STATIC_INLINE_T StringDataW* StringT_getDEmpty<CharW>() { return &StringW_dEmpty; }

// ============================================================================
// [Fog::String - Helpers]
// ============================================================================

static FOG_INLINE void StringT_chcopy(char* dst, const char* src, size_t length)
{
  for (size_t i = 0; i < length; i++)
    dst[i] = src[i];
}

static FOG_INLINE void StringT_chcopy(CharW* dst, const char* src, size_t length)
{
  for (size_t i = 0; i < length; i++)
    dst[i] = src[i];
}

static FOG_INLINE void StringT_chcopy(CharW* dst, const CharW* src, size_t length)
{
  for (size_t i = 0; i < length; i++)
    dst[i] = src[i];
}

static FOG_INLINE void StringW_chcopy_localized(CharW* dst, const char* src, CharW offset, size_t length)
{
  for (size_t i = 0; i < length; i++)
  {
    CharW c = CharW(src[i]);
    if (c.isAsciiDigit()) c += offset;
    dst[i] = c;
  }
}

static FOG_INLINE void StringW_chcopy_localized(CharW* dst, const CharW* src, CharW offset, size_t length)
{
  for (size_t i = 0; i < length; i++)
  {
    CharW c = CharW(src[i]);
    if (c.isAsciiDigit()) c += offset;
    dst[i] = c;
  }
}

static FOG_INLINE void StringT_chmove(char* dst, const char* src, size_t length)
{
  if (dst <= src)
    StringT_chcopy(dst, src, length);

  size_t i = length;
  while (i)
  {
    i--;
    dst[i] = src[i];
  }
}

static FOG_INLINE void StringT_chmove(CharW* dst, const CharW* src, size_t length)
{
  if (dst <= src)
    StringT_chcopy(dst, src, length);

  size_t i = length;
  while (i)
  {
    i--;
    dst[i] = src[i];
  }
}

static FOG_INLINE void StringT_chfill(char* dst, char ch, size_t length)
{
  for (size_t i = 0; i < length; i++)
    dst[i] = ch;
}

static FOG_INLINE void StringT_chfill(CharW* dst, uint16_t ch, size_t length)
{
  for (size_t i = 0; i < length; i++)
    dst[i] = ch;
}

static FOG_INLINE size_t StringT_cheq(const char* a, const char* b, size_t length)
{
  size_t i;
  for (i = 0; i < length; i++)
    if (a[i] != b[i])
      break;
  return i;
}

static FOG_INLINE size_t StringT_cheq(const CharW* a, const char* b, size_t length)
{
  size_t i;
  for (i = 0; i < length; i++)
    if (a[i] != (unsigned char)b[i])
      break;
  return i;
}

static FOG_INLINE size_t StringT_cheq(const CharW* a, const CharW* b, size_t length)
{
  size_t i;
  for (i = 0; i < length; i++)
    if (a[i] != b[i])
      break;
  return i;
}

static FOG_INLINE size_t StringT_cheqi(const char* a, const char* b, size_t length)
{
  size_t i;
  for (i = 0; i < length; i++)
    if (CharA::toLower(a[i]) != CharA::toLower(b[i]))
      break;
  return i;
}

static FOG_INLINE size_t StringT_cheqi(const CharW* a, const char* b, size_t length)
{
  size_t i;
  for (i = 0; i < length; i++)
    if (CharW::toLower(a[i]) != CharA::toLower((unsigned char)b[i]))
      break;
  return i;
}

static FOG_INLINE size_t StringT_cheqi(const CharW* a, const CharW* b, size_t length)
{
  size_t i;
  for (i = 0; i < length; i++)
    if (CharW::toLower(a[i]) != CharW::toLower(b[i]))
      break;
  return i;
}

template<typename ValueT, typename CharT>
FOG_STATIC_INLINE_T uint StringT_getCaseInsensitiveChars(ValueT* ciArray, CharT ch)
{
  return 0;
}

template<>
FOG_STATIC_INLINE_T uint StringT_getCaseInsensitiveChars<char, char>(char* ciArray, char ch)
{
  ciArray[0] = CharA::toLower(ch);
  ciArray[1] = CharA::toUpper(ch);

  uint ciCount = 2;
  if (ciArray[0] == ciArray[1])
    ciCount--;
  return ciCount;
}

template<>
FOG_STATIC_INLINE_T uint StringT_getCaseInsensitiveChars<uint16_t, CharW>(uint16_t* ciArray, CharW ch)
{
  ciArray[0] = (uint16_t)CharW::toLower(ch);
  ciArray[1] = (uint16_t)CharW::toUpper(ch);
  ciArray[2] = (uint16_t)CharW::toTitle(ch);

  uint ciCount = 3;
  if (ciArray[1] == ciArray[2])
    ciCount--;
  if (ciArray[0] == ciArray[1])
    ciCount--;
  return ciCount;
}

// ============================================================================
// [Fog::String - Construction / Destruction]
// ============================================================================

template<typename CharT>
static void FOG_CDECL StringT_ctor(CharT_(String)* self)
{
  self->_d = StringT_getDEmpty<CharT>()->addRef();
}

template<typename CharT>
static void FOG_CDECL StringT_ctorCopy(CharT_(String)* self, const CharT_(String)* other)
{
  self->_d = other->_d->addRef();
}

template<typename CharT, typename SrcT>
static void FOG_CDECL StringT_ctorStub(CharT_(String)* self, const SrcT_(Stub)* stub)
{
  CharT_(StringData)* d = CharI_(String)::_dCreate(0, *stub);

  if (FOG_IS_NULL(d))
    d = StringT_getDEmpty<CharT>()->addRef();

  self->_d = d;
}

static void FOG_CDECL StringW_ctorCodec(StringW* self, const StubA* stub, const TextCodec* tc)
{
  self->_d = StringT_getDEmpty<CharW>()->addRef();
  tc->decode(*self, *stub);
}

template<typename CharT>
static void FOG_CDECL StringT_ctorStringString(CharT_(String)* self, const CharT_(String)* a, const CharT_(String)* b)
{
  size_t aLength = a->getLength();
  size_t bLength = b->getLength();

  CharT_(StringData)* d;

  if (bLength == 0)
  {
    d = a->_d->addRef();
  }
  else
  {
    if (FOG_LIKELY(Math::canSum(aLength, bLength)))
    {
      size_t length = aLength + bLength;

      d = CharI_(String)::_dCreate(length);
      if (FOG_IS_NULL(d))
        goto _Fail;

      d->length = length;
      StringT_chcopy(d->data          , a->getData(), aLength);
      StringT_chcopy(d->data + aLength, b->getData(), bLength);
      d->data[length] = 0;
    }
    else
    {
_Fail:
      d = StringT_getDEmpty<CharT>()->addRef();
    }
  }

  self->_d = d;
}

template<typename CharT, typename SrcT>
static void FOG_CDECL StringT_ctorStringStub(CharT_(String)* self, const CharT_(String)* a, const SrcT_(Stub)* b)
{
  size_t aLength = a->getLength();
  size_t bLength = b->getComputedLength();

  CharT_(StringData)* d;

  if (bLength == 0)
  {
    d = a->_d->addRef();
  }
  else
  {
    if (FOG_LIKELY(Math::canSum(aLength, bLength)))
    {
      size_t length = aLength + bLength;

      d = CharI_(String)::_dCreate(length);
      if (FOG_IS_NULL(d))
        goto _Fail;

      d->length = length;
      StringT_chcopy(d->data          , a->getData(), aLength);
      StringT_chcopy(d->data + aLength, b->getData(), bLength);
      d->data[length] = 0;
    }
    else
    {
_Fail:
      d = StringT_getDEmpty<CharT>()->addRef();
    }
  }

  self->_d = d;
}

template<typename CharT, typename SrcT>
static void FOG_CDECL StringT_ctorStubString(CharT_(String)* self, const SrcT_(Stub)* a, const CharT_(String)* b)
{
  size_t aLength = a->getComputedLength();
  size_t bLength = b->getLength();

  CharT_(StringData)* d;

  if (FOG_LIKELY(Math::canSum(aLength, bLength)))
  {
    size_t length = aLength + bLength;

    d = CharI_(String)::_dCreate(length);
    if (FOG_IS_NULL(d))
      goto _Fail;

    d->length = length;
    StringT_chcopy(d->data          , a->getData(), aLength);
    StringT_chcopy(d->data + aLength, b->getData(), bLength);
    d->data[length] = 0;
  }
  else
  {
_Fail:
    d = StringT_getDEmpty<CharT>()->addRef();
  }

  self->_d = d;
}

template<typename CharT, typename SrcT>
static void FOG_CDECL StringT_ctorStubStub(CharT_(String)* self, const SrcT_(Stub)* a, const SrcT_(Stub)* b)
{
  size_t aLength = a->getComputedLength();
  size_t bLength = b->getComputedLength();

  CharT_(StringData)* d;

  if (FOG_LIKELY(Math::canSum(aLength, bLength)))
  {
    size_t length = aLength + bLength;

    d = CharI_(String)::_dCreate(length);
    if (FOG_IS_NULL(d))
      goto _Fail;

    d->length = length;
    StringT_chcopy(d->data          , a->getData(), aLength);
    StringT_chcopy(d->data + aLength, b->getData(), bLength);
    d->data[length] = 0;
  }
  else
  {
_Fail:
    d = StringT_getDEmpty<CharT>()->addRef();
  }

  self->_d = d;
}

template<typename CharT>
static void FOG_CDECL StringT_ctorSubstr(CharT_(String)* self, const CharT_(String)* other, const Range* range)
{
  CharT_(StringData)* d;

  size_t rStart, rEnd;
  size_t oLength = other->getLength();

  if (Range::fit(rStart, rEnd, oLength, range))
  {
    size_t rLength = rEnd - rStart;
    d = CharI_(String)::_dCreate(rLength, CharT_(Stub)(other->getData() + rStart, rLength));

    if (FOG_IS_NULL(d))
      goto _Fail;
  }
  else
  {
_Fail:
    d = StringT_getDEmpty<CharT>()->addRef();
  }

  self->_d = d;
}

template<typename CharT>
static void FOG_CDECL StringT_ctorU32(CharT_(String)* self, uint32_t n, bool isUnsigned)
{
  char nBuffer[32];
  char* nPtr = nBuffer + FOG_ARRAY_SIZE(nBuffer);

  if (!isUnsigned)
  {
    if ((int32_t)n < 0)
      n = -(int32_t)n;
    else
      isUnsigned = true;
  }

  do {
    uint32_t nDiv = n / 10;
    uint32_t nRem = n % 10;

    *--nPtr = (unsigned char)'0' + (unsigned char)(nRem);
    n = nDiv;
  } while (n != 0);

  if (!isUnsigned)
    *--nPtr = '-';

  size_t nLength = (size_t)(nBuffer + FOG_ARRAY_SIZE(nBuffer) - nPtr);
  CharT_(StringData)* d = CharI_(String)::_dCreate(nLength, Ascii8(nPtr, nLength));

  if (FOG_IS_NULL(d))
    d = StringT_getDEmpty<CharT>()->addRef();

  self->_d = d;
}

template<typename CharT>
static void FOG_CDECL StringT_ctorU64(CharT_(String)* self, uint64_t n, bool isUnsigned)
{
  char nBuffer[32];
  char* nPtr = nBuffer + FOG_ARRAY_SIZE(nBuffer);

  if (!isUnsigned)
  {
    if ((int64_t)n < 0)
      n = -(int64_t)n;
    else
      isUnsigned = true;
  }

  do {
    uint64_t nDiv = n / FOG_UINT64_C(10);
    uint64_t nRem = n % FOG_UINT64_C(10);

    *--nPtr = (unsigned char)'0' + (unsigned char)(nRem);
    n = nDiv;
  } while (n != 0);

  if (!isUnsigned)
    *--nPtr = '-';

  size_t nLength = (size_t)(nBuffer + FOG_ARRAY_SIZE(nBuffer) - nPtr);
  CharT_(StringData)* d = CharI_(String)::_dCreate(nLength, Ascii8(nPtr, nLength));

  if (FOG_IS_NULL(d))
    d = StringT_getDEmpty<CharT>()->addRef();

  self->_d = d;
}

template<typename CharT>
static void FOG_CDECL StringT_ctorDouble(CharT_(String)* self, double d)
{
  self->_d = StringT_getDEmpty<CharT>()->addRef();
  self->setReal(d);
}

template<typename CharT>
static void FOG_CDECL StringT_dtor(CharT_(String)* self)
{
  CharT_(StringData)* d = self->_d;

  if (d != NULL)
    d->release();
}

// ============================================================================
// [Fog::String - Sharing]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_detach(CharT_(String)* self)
{
  CharT_(StringData)* d = self->_d;
  if (d->reference.get() == 1)
    return ERR_OK;

  d = CharI_(String)::_dCreate(d->length, CharT_(Stub)(d->data, d->length));
  if (FOG_IS_NULL(d))
    return ERR_RT_OUT_OF_MEMORY;

  atomicPtrXchg(&self->_d, d)->release();
  return ERR_OK;
}

// ============================================================================
// [Fog::String - Container]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_reserve(CharT_(String)* self, size_t capacity)
{
  CharT_(StringData)* d = self->_d;
  size_t length = d->length;

  if (capacity < length)
    capacity = length;

  if (d->reference.get() > 1)
  {
    d = CharI_(String)::_dCreate(capacity, CharT_(Stub)(d->data, d->length));
    if (FOG_IS_NULL(d))
      return ERR_RT_OUT_OF_MEMORY;

    atomicPtrXchg(&self->_d, d)->release();
  }
  else if (d->capacity < capacity)
  {
    d = CharI_(String)::_dRealloc(d, capacity);
    if (FOG_IS_NULL(d))
      return ERR_RT_OUT_OF_MEMORY;

    self->_d = d;
  }

  return ERR_OK;
}

template<typename CharT>
static err_t FOG_CDECL StringT_resize(CharT_(String)* self, size_t len)
{
  CharT_(StringData)* d = self->_d;

  if (d->reference.get() > 1)
  {
    d = CharI_(String)::_dCreate(len, CharT_(Stub)(d->data, Math::min(len, d->length)));
    if (FOG_IS_NULL(d))
      return ERR_RT_OUT_OF_MEMORY;

    atomicPtrXchg(&self->_d, d)->release();
  }
  else if (d->capacity < len)
  {
    d = CharI_(String)::_dRealloc(d, len);
    if (FOG_IS_NULL(d))
      return ERR_RT_OUT_OF_MEMORY;

    self->_d = d;
  }

  d->hashCode = 0;
  d->length = len;
  d->data[len] = 0;
  return ERR_OK;
}

template<typename CharT>
static err_t FOG_CDECL StringT_truncate(CharT_(String)* self, size_t len)
{
  if (self->_d->length <= len)
    return ERR_OK;
  else
    return self->resize(len);
}

template<typename CharT>
static void FOG_CDECL StringT_squeeze(CharT_(String)* self)
{
  CharT_(StringData)* d = self->_d;
  size_t i = d->length;
  size_t c = d->capacity;

  // Pad to 8 or 16 bytes.
  i = (i + 7) & ~7;
  if (i >= c)
    return;

  d = CharI_(String)::_dCreate(0, CharT_(Stub)(d->data, d->length));
  if (FOG_IS_NULL(d))
    return;

  atomicPtrXchg(&self->_d, d)->release();
}

template<typename CharT>
static CharT* FOG_CDECL StringT_prepare(CharT_(String)* self, uint32_t cntOp, size_t length)
{
  CharT_(StringData)* d = self->_d;

  if (cntOp == CONTAINER_OP_REPLACE)
  {
    if (d->reference.get() == 1 && length <= d->capacity)
    {
      d->hashCode = 0;
      d->length = length;
      d->data[length] = 0;
      return d->data;
    }

    d = CharI_(String)::_dCreate(length);
    if (FOG_IS_NULL(d))
      return NULL;
    atomicPtrXchg(&self->_d, d)->release();

    d->length = length;
    d->data[length] = 0;
    return d->data;
  }
  else
  {
    size_t before = d->length;
    size_t after = before + length;

    // Overflow.
    if (before > after)
      return NULL;

    if (d->reference.get() > 1)
    {
      size_t optimal = ContainerUtil::getGrowCapacity(sizeof(CharT_(StringData)), sizeof(CharT), before, after);

      d = CharI_(String)::_dCreate(optimal, CharT_(Stub)(d->data, before));
      if (FOG_IS_NULL(d))
        return NULL;
      atomicPtrXchg(&self->_d, d)->release();
    }
    else if (d->capacity < after)
    {
      size_t optimal = ContainerUtil::getGrowCapacity(sizeof(CharT_(StringData)), sizeof(CharT), before, after);

      d = CharI_(String)::_dRealloc(d, optimal);
      if (FOG_IS_NULL(d))
        return NULL;
      self->_d = d;
    }

    d->hashCode = 0;
    d->length = after;
    d->data[after] = 0;
    return d->data + before;
  }
}

template<typename CharT>
static CharT* FOG_CDECL StringT_add(CharT_(String)* self, size_t length)
{
  CharT_(StringData)* d = self->_d;

  size_t before = d->length;
  size_t after = before + length;

  // Overflow.
  if (before > after)
    return NULL;

  if (d->reference.get() > 1)
  {
    size_t optimal = ContainerUtil::getGrowCapacity(sizeof(CharT_(StringData)), sizeof(CharT), before, after);

    d = CharI_(String)::_dCreate(optimal, CharT_(Stub)(d->data, before));
    if (FOG_IS_NULL(d))
      return NULL;
    atomicPtrXchg(&self->_d, d)->release();
  }
  else if (d->capacity < after)
  {
    size_t optimal = ContainerUtil::getGrowCapacity(sizeof(CharT_(StringData)), sizeof(CharT), before, after);

    d = CharI_(String)::_dRealloc(d, optimal);
    if (FOG_IS_NULL(d))
      return NULL;
    self->_d = d;
  }

  d->hashCode = 0;
  d->length = after;
  d->data[after] = 0;
  return d->data + before;
}

// ============================================================================
// [Fog::String - Clear / Reset]
// ============================================================================

template<typename CharT>
static void FOG_CDECL StringT_clear(CharT_(String)* self)
{
  CharT_(StringData)* d = self->_d;

  if (d->reference.get() > 1)
  {
    atomicPtrXchg(&self->_d, StringT_getDEmpty<CharT>()->addRef())->release();
    return;
  }

  d->hashCode = 0;
  d->length = 0;
  d->data[0] = 0;
}

template<typename CharT>
static void FOG_CDECL StringT_reset(CharT_(String)* self)
{
  atomicPtrXchg(&self->_d, StringT_getDEmpty<CharT>()->addRef())->release();
}

// ============================================================================
// [Fog::String - HashCode]
// ============================================================================

template<typename CharT>
static uint32_t FOG_CDECL StringT_getHashCode(const CharT_(String)* self)
{
  CharT_(StringData)* d = self->_d;
  uint32_t hashCode = d->hashCode;

  if (hashCode != 0)
    return hashCode;

  hashCode = d->hashCode = HashUtil::hash<CharT_(Stub)>(CharT_(Stub)(d->data, d->length));
  return hashCode;
}

// ============================================================================
// [Fog::String - Set]
// ============================================================================

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_setStub(CharT_(String)* self, const SrcT_(Stub)* stub)
{
  const SrcT* sData = stub->getData();
  size_t sLength = stub->getComputedLength();

  CharT* dst = self->_prepare(CONTAINER_OP_REPLACE, sLength);
  if (FOG_IS_NULL(dst))
    return ERR_RT_OUT_OF_MEMORY;

  StringT_chcopy(dst, sData, sLength);
  return ERR_OK;
}

static err_t FOG_CDECL StringW_setStubCodec(StringW* self, const StubA* stub, const TextCodec* tc)
{
  return tc->decode(*self, *stub, NULL, CONTAINER_OP_REPLACE);
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_setString(CharT_(String)* self, const SrcT_(String)* other)
{
  if (sizeof(CharT) == sizeof(SrcT))
  {
    atomicPtrXchg(&self->_d,
      reinterpret_cast<CharT_(StringData)*>(other->_d)->addRef())->release();
    return ERR_OK;
  }
  else
  {
    const SrcT* sData = other->getData();
    size_t sLength = other->getLength();

    CharT* dst = self->_prepare(CONTAINER_OP_REPLACE, sLength);
    if (FOG_IS_NULL(dst))
      return ERR_RT_OUT_OF_MEMORY;

    StringT_chcopy(dst, sData, sLength);
    return ERR_OK;
  }
}

static err_t FOG_CDECL StringW_setStringCodec(StringW* self, const StringA* other, const TextCodec* tc)
{
  return tc->decode(*self, *other, NULL, CONTAINER_OP_REPLACE);
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_setStringEx(CharT_(String)* self, const SrcT_(String)* other, const Range* range)
{
  const SrcT* sData = other->getData();
  size_t sLength = other->getLength();

  if (sizeof(CharT) == sizeof(SrcT) && (const void*)self == (const void*)other)
    return self->slice(*range);

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, sLength, range))
  {
    self->clear();
    return ERR_OK;
  }

  size_t rLength = rEnd - rStart;
  CharT* dst = self->_prepare(CONTAINER_OP_REPLACE, rLength);
  if (FOG_IS_NULL(dst))
    return ERR_RT_OUT_OF_MEMORY;

  StringT_chcopy(dst, sData + rStart, rLength);
  return ERR_OK;
}

template<typename CharT>
static err_t FOG_CDECL StringT_setDeep(CharT_(String)* self, const CharT_(String)* other)
{
  if (self == other)
    return ERR_OK;

  const CharT* sData = other->getData();
  size_t sLength = other->getLength();

  CharT* p = self->_prepare(CONTAINER_OP_REPLACE, sLength);
  if (FOG_IS_NULL(p))
    return ERR_RT_OUT_OF_MEMORY;

  StringT_chcopy(p, sData, sLength);
  return ERR_OK;
}

// ============================================================================
// [Fog::String - Append]
// ============================================================================

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_appendStub(CharT_(String)* self, const SrcT_(Stub)* stub)
{
  const SrcT* sData = stub->getData();
  size_t sLength = stub->getComputedLength();

  CharT* dst = self->_prepare(CONTAINER_OP_APPEND, sLength);
  if (FOG_IS_NULL(dst))
    return ERR_RT_OUT_OF_MEMORY;

  StringT_chcopy(dst, sData, sLength);
  return ERR_OK;
}

static err_t FOG_CDECL StringW_appendStubCodec(StringW* self, const StubA* stub, const TextCodec* tc)
{
  return tc->decode(*self, *stub, NULL, CONTAINER_OP_APPEND);
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_appendString(CharT_(String)* self, const SrcT_(String)* other)
{
  const SrcT* sData = other->getData();
  size_t sLength = other->getLength();

  CharT* dst = self->_prepare(CONTAINER_OP_APPEND, sLength);
  if (FOG_IS_NULL(dst))
    return ERR_RT_OUT_OF_MEMORY;

  // Update the sData in case that self == other, because StringX::_prepare() can
  // invalidate the sData pointer.
  if (sizeof(CharT) == sizeof(SrcT) &&
      reinterpret_cast<const void*>(self) == reinterpret_cast<const void*>(other))
  {
    sData = other->_d->data;
  }

  StringT_chcopy(dst, sData, sLength);
  return ERR_OK;
}

static err_t FOG_CDECL StringW_appendStringCodec(StringW* self, const StringA* other, const TextCodec* tc)
{
  return tc->decode(*self, *other, NULL, CONTAINER_OP_REPLACE);
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_appendStringEx(CharT_(String)* self, const SrcT_(String)* other, const Range* range)
{
  const SrcT* sData = other->getData();
  size_t sLength = other->getLength();
  size_t rStart, rEnd;

  if (!Range::fit(rStart, rEnd, sLength, range))
    return ERR_OK;

  size_t rLength = rEnd - rStart;
  CharT* dst = self->_prepare(CONTAINER_OP_APPEND, rLength);
  if (FOG_IS_NULL(dst))
    return ERR_RT_OUT_OF_MEMORY;

  if (sizeof(CharT) == sizeof(SrcT) && (const void*)self == (const void*)other)
    sData = other->_d->data;

  StringT_chcopy(dst, sData + rStart, rLength);
  return ERR_OK;
}

// ============================================================================
// [Fog::String - OpFill]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_opFill(CharT_(String)* self, uint32_t cntOp, CharT_Type ch, size_t length)
{
  CharT* p = self->_prepare(cntOp, length);

  if (FOG_IS_NULL(p))
    return ERR_RT_OUT_OF_MEMORY;

  StringT_chfill(p, ch, length);
  return ERR_OK;
}

// ============================================================================
// [Fog::String - OpBool]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_opBool(CharT_(String)* self, uint32_t cntOp, bool b)
{
  if (b)
    return self->set(Ascii8("true", 4));
  else
    return self->set(Ascii8("false", 5));
}

// ============================================================================
// [Fog::String - OpInt]
// ============================================================================

template<typename CharT>
static err_t StringT_appendNTOA(CharT_(String)* self, uint32_t cntOp, uint64_t n, const FormatInt* fmt, NTOAContext* ctx)
{
  CharT prefixBuffer[4];
  CharT* prefix = prefixBuffer;

  uint32_t flags = fmt->getFlags();

  if (ctx->negative)
    *prefix++ = '-';
  else if (flags & STRING_FORMAT_SIGN)
    *prefix++ = '+';
  else if (flags & STRING_FORMAT_BLANK)
    *prefix++ = ' ';

  if (flags & STRING_FORMAT_ALTERNATE)
  {
    uint32_t base = fmt->getBase();

    if (base == 8)
    {
      if (n != 0)
      {
        *--ctx->result = '0';
        ctx->length++;
      }
    }
    else if (base == 16)
    {
      *prefix++ = CharT('0');
      *prefix++ = CharT((flags & STRING_FORMAT_CAPITALIZE_E_OR_X) ? 'X' : 'x');
    }
  }

  size_t prefixLength = (size_t)(prefix - prefixBuffer);
  size_t resultLength = ctx->length;

  size_t width = fmt->getWidth();
  size_t precision = fmt->getPrecision();

  if (width == NO_WIDTH)
    width = 0;

  if ((flags & STRING_FORMAT_ZERO_PAD) != 0 && precision == NO_PRECISION && width > prefixLength + resultLength)
    precision = width - prefixLength;

  if (precision == NO_PRECISION)
    precision = 0;

  size_t fillLength = (resultLength < precision) ? precision - resultLength : 0;
  size_t finalLength = prefixLength + resultLength + fillLength;
  size_t widthLength = (finalLength < width) ? width - finalLength : 0;

  finalLength += widthLength;

  CharT* p = self->_prepare(cntOp, finalLength);
  if (FOG_IS_NULL(p))
    return ERR_RT_OUT_OF_MEMORY;

  // Left justification.
  if (!(flags & STRING_FORMAT_LEFT))
  {
    StringT_chfill(p, CharT(' '), widthLength);
    p += widthLength;
  }

  // Number with prefix and precision.
  StringT_chcopy(p, prefixBuffer, prefixLength);
  p += prefixLength;

  // Body.
  StringT_chfill(p, CharT('0'), fillLength);
  p += fillLength;

  StringT_chcopy(p, ctx->result, resultLength);
  p += resultLength;

  // Right justification.
  if (flags & STRING_FORMAT_LEFT)
  {
    StringT_chfill(p, CharT(' '), widthLength);
  }

  return ERR_OK;
}

static err_t StringW_appendNTOA(StringW* self, uint32_t cntOp, uint64_t n, const FormatInt* fmt, const Locale* locale, NTOAContext* ctx)
{
  // If no locale was specified then run using common fast-path, instead of
  // using POSIX Fog::Locale instance.
  if (locale == NULL)
    return StringT_appendNTOA<CharW>(self, cntOp, n, fmt, ctx);

  CharW zero = locale->getChar(LOCALE_CHAR_ZERO) - CharW('0');

  CharW prefixBuffer[4];
  CharW* prefix = prefixBuffer;

  uint32_t flags = fmt->getFlags();

  if (ctx->negative)
    *prefix++ = locale->getChar(LOCALE_CHAR_MINUS);
  else if (flags & STRING_FORMAT_SIGN)
    *prefix++ = locale->getChar(LOCALE_CHAR_PLUS);
  else if (flags & STRING_FORMAT_BLANK)
    *prefix++ = CharW(' ');

  if (flags & STRING_FORMAT_ALTERNATE)
  {
    uint32_t base = fmt->getBase();

    if (base == 8)
    {
      if (n != 0)
      {
        *--ctx->result = '0';
        ctx->length++;
      }
    }
    else if (base == 16)
    {
      *prefix++ = CharW('0') + zero;
      *prefix++ = CharW((flags & STRING_FORMAT_CAPITALIZE_E_OR_X) ? 'X' : 'x');
    }
  }

  size_t prefixLength = (size_t)(prefix - prefixBuffer);
  size_t resultLength = ctx->length;

  size_t width = fmt->getWidth();
  size_t precision = fmt->getPrecision();

  if (width == NO_WIDTH)
    width = 0;

  if ((flags & STRING_FORMAT_ZERO_PAD) != 0 && precision == NO_PRECISION && width > prefixLength + resultLength)
    precision = width - prefixLength;

  if (precision == NO_PRECISION)
    precision = 0;

  size_t fillLength = (resultLength < precision) ? precision - resultLength : 0;
  size_t finalLength = prefixLength + resultLength + fillLength;
  size_t widthLength = (finalLength < width) ? width - finalLength : 0;

  finalLength += widthLength;

  CharW* p = self->_prepare(cntOp, finalLength);
  if (FOG_IS_NULL(p))
    return ERR_RT_OUT_OF_MEMORY;

  // Left justification.
  if (!(flags & STRING_FORMAT_LEFT))
  {
    StringT_chfill(p, ' ', widthLength);
    p += widthLength;
  }

  // Number with prefix and precision.
  StringT_chcopy(p, prefixBuffer, prefixLength);
  p += prefixLength;

  // Body.
  StringT_chfill(p, zero + '0', fillLength);
  p += fillLength;

  StringW_chcopy_localized(p, ctx->result, zero, resultLength);
  p += resultLength;

  // Right justification.
  if (flags & STRING_FORMAT_LEFT)
  {
    StringT_chfill(p, ' ', widthLength);
  }

  return ERR_OK;
}

template<typename CharT>
static err_t FOG_CDECL StringT_opI32(CharT_(String)* self, uint32_t cntOp, int32_t n)
{
  char nBuffer[32];
  char* nPtr = nBuffer + FOG_ARRAY_SIZE(nBuffer);

  uint32_t u = (n >= 0) ? n : -n;
  do {
    uint32_t uDiv = u / 10;
    uint32_t uRem = u % 10;

    *--nPtr = (unsigned char)'0' + (unsigned char)(uRem);
    u = uDiv;
  } while (u != 0);

  if (n < 0)
    *--nPtr = '-';

  size_t nLength = (size_t)(nBuffer + FOG_ARRAY_SIZE(nBuffer) - nPtr);
  CharT* dst = self->_prepare(cntOp, nLength);

  if (FOG_IS_NULL(dst))
    return ERR_RT_OUT_OF_MEMORY;

  StringT_chcopy(dst, nPtr, nLength);
  return ERR_OK;
}

static err_t FOG_CDECL StringA_opI32Ex(StringA* self, uint32_t cntOp, int32_t n, const FormatInt* fmt)
{
  NTOAContext ctx;
  StringUtil::itoa(&ctx, n, fmt->getBase(), (fmt->getFlags() & STRING_FORMAT_CAPITALIZE) ? TEXT_CASE_UPPER : TEXT_CASE_LOWER);

  return StringT_appendNTOA<char>(self, cntOp, n, fmt, &ctx);
}

static err_t FOG_CDECL StringW_opI32Ex(StringW* self, uint32_t cntOp, int32_t n, const FormatInt* fmt, const Locale* locale)
{
  NTOAContext ctx;
  StringUtil::itoa(&ctx, n, fmt->getBase(), (fmt->getFlags() & STRING_FORMAT_CAPITALIZE) ? TEXT_CASE_UPPER : TEXT_CASE_LOWER);

  return StringW_appendNTOA(self, cntOp, n, fmt, locale, &ctx);
}

template<typename CharT>
static err_t FOG_CDECL StringT_opU32(CharT_(String)* self, uint32_t cntOp, uint32_t n)
{
  char nBuffer[32];
  char* nPtr = nBuffer + FOG_ARRAY_SIZE(nBuffer);

  uint32_t u = n;
  do {
    uint32_t uDiv = u / 10;
    uint32_t uRem = u % 10;

    *--nPtr = (unsigned char)'0' + (unsigned char)(uRem);
    u = uDiv;
  } while (u != 0);

  size_t nLength = (size_t)(nBuffer + FOG_ARRAY_SIZE(nBuffer) - nPtr);
  CharT* dst = self->_prepare(cntOp, nLength);

  if (FOG_IS_NULL(dst))
    return ERR_RT_OUT_OF_MEMORY;

  StringT_chcopy(dst, nPtr, nLength);
  return ERR_OK;
}

static err_t FOG_CDECL StringA_opU32Ex(StringA* self, uint32_t cntOp, uint32_t n, const FormatInt* fmt)
{
  NTOAContext ctx;
  StringUtil::utoa(&ctx, n, fmt->getBase(), (fmt->getFlags() & STRING_FORMAT_CAPITALIZE) ? TEXT_CASE_UPPER : TEXT_CASE_LOWER);

  return StringT_appendNTOA<char>(self, cntOp, n, fmt, &ctx);
}

static err_t FOG_CDECL StringW_opU32Ex(StringW* self, uint32_t cntOp, uint32_t n, const FormatInt* fmt, const Locale* locale)
{
  NTOAContext ctx;
  StringUtil::utoa(&ctx, n, fmt->getBase(), (fmt->getFlags() & STRING_FORMAT_CAPITALIZE) ? TEXT_CASE_UPPER : TEXT_CASE_LOWER);

  return StringW_appendNTOA(self, cntOp, n, fmt, locale, &ctx);
}

template<typename CharT>
static err_t FOG_CDECL StringT_opI64(CharT_(String)* self, uint32_t cntOp, int64_t n)
{
  char nBuffer[32];
  char* nPtr = nBuffer + FOG_ARRAY_SIZE(nBuffer);

  uint64_t u = (n >= 0) ? n : -n;
  do {
    uint64_t uDiv = u / FOG_UINT64_C(10);
    uint64_t uRem = u % FOG_UINT64_C(10);

    *--nPtr = (unsigned char)'0' + (unsigned char)(uRem);
    u = uDiv;
  } while (u != 0);

  if (n < 0)
    *--nPtr = '-';

  size_t nLength = (size_t)(nBuffer + FOG_ARRAY_SIZE(nBuffer) - nPtr);
  CharT* dst = self->_prepare(cntOp, nLength);

  if (FOG_IS_NULL(dst))
    return ERR_RT_OUT_OF_MEMORY;

  StringT_chcopy(dst, nPtr, nLength);
  return ERR_OK;
}

static err_t FOG_CDECL StringA_opI64Ex(StringA* self, uint32_t cntOp, int64_t n, const FormatInt* fmt)
{
  NTOAContext ctx;
  StringUtil::itoa(&ctx, n, fmt->getBase(), (fmt->getFlags() & STRING_FORMAT_CAPITALIZE) ? TEXT_CASE_UPPER : TEXT_CASE_LOWER);

  return StringT_appendNTOA<char>(self, cntOp, n, fmt, &ctx);
}

static err_t FOG_CDECL StringW_opI64Ex(StringW* self, uint32_t cntOp, int64_t n, const FormatInt* fmt, const Locale* locale)
{
  NTOAContext ctx;
  StringUtil::itoa(&ctx, n, fmt->getBase(), (fmt->getFlags() & STRING_FORMAT_CAPITALIZE) ? TEXT_CASE_UPPER : TEXT_CASE_LOWER);

  return StringW_appendNTOA(self, cntOp, n, fmt, locale, &ctx);
}

template<typename CharT>
static err_t FOG_CDECL StringT_opU64(CharT_(String)* self, uint32_t cntOp, uint64_t n)
{
  char nBuffer[32];
  char* nPtr = nBuffer + FOG_ARRAY_SIZE(nBuffer);

  uint64_t u = n;
  do {
    uint64_t uDiv = u / FOG_UINT64_C(10);
    uint64_t uRem = u % FOG_UINT64_C(10);

    *--nPtr = (unsigned char)'0' + (unsigned char)(uRem);
    u = uDiv;
  } while (u != 0);

  size_t nLength = (size_t)(nBuffer + FOG_ARRAY_SIZE(nBuffer) - nPtr);
  CharT* dst = self->_prepare(cntOp, nLength);

  if (FOG_IS_NULL(dst))
    return ERR_RT_OUT_OF_MEMORY;

  StringT_chcopy(dst, nPtr, nLength);
  return ERR_OK;
}

static err_t FOG_CDECL StringA_opU64Ex(StringA* self, uint32_t cntOp, uint64_t n, const FormatInt* fmt)
{
  NTOAContext ctx;
  StringUtil::utoa(&ctx, n, fmt->getBase(), (fmt->getFlags() & STRING_FORMAT_CAPITALIZE) ? TEXT_CASE_UPPER : TEXT_CASE_LOWER);

  return StringT_appendNTOA<char>(self, cntOp, n, fmt, &ctx);
}

static err_t FOG_CDECL StringW_opU64Ex(StringW* self, uint32_t cntOp, uint64_t n, const FormatInt* fmt, const Locale* locale)
{
  NTOAContext ctx;
  StringUtil::utoa(&ctx, n, fmt->getBase(), (fmt->getFlags() & STRING_FORMAT_CAPITALIZE) ? TEXT_CASE_UPPER : TEXT_CASE_LOWER);

  return StringW_appendNTOA(self, cntOp, n, fmt, locale, &ctx);
}

// ============================================================================
// [Fog::String - OpFloat / OpDouble]
// ============================================================================

template<typename CharT>
static CharT* StringT_appendExponent(CharT* p, uint exp, CharT zero)
{
  uint t;

  if (exp > 99)
  {
    t = exp / 100;

    *p++ = zero + CharT(t);
    exp -= t * 100;
  }

  t = exp / 10;
  *p++ = zero + CharT(t);

  exp -= t * 10;
  *p++ = zero + CharT(exp);

  return p;
}

#if 0
err_t StringA::appendDouble(double d, int form, const FormatParams& ff)
{
  err_t err = ERR_OK;

  StringUtil::NTOAOut out;

  size_t width = ff.width;
  size_t precision = ff.precision;
  uint32_t fmt = ff.flags;

  size_t beginLength = _d->length;
  size_t numberLength;
  size_t i;
  size_t savedPrecision = precision;
  int decpt;

  char* bufCur;
  char* bufEnd;

  char* dest;
  char sign = 0;

  if (precision == NO_PRECISION) precision = 6;

  if (d < 0.0)
    { sign = '-'; d = -d; }
  else if (fmt & STRING_FORMAT_SIGN)
    sign = '+';
  else if (fmt & STRING_FORMAT_BLANK)
    sign = ' ';

  if (sign != 0) append(sign);

  // Decimal form.
  if (doubleForm == DF_DECIMAL)
  {
    StringUtil::dtoa(d, 3, (uint32_t)precision, &out);

    decpt = out.decpt;
    if (out.decpt == 9999) goto _InfOrNaN;

    bufCur = out.result;
    bufEnd = bufCur + out.length;

    // Reserve some space for number.
    i = precision + 16;
    if (decpt > 0) i += (size_t)decpt;

    dest = beginManipulation(i, CONTAINER_OP_APPEND);
    if (FOG_IS_NULL(dest)) { err = ERR_RT_OUT_OF_MEMORY; goto _Ret; }

    while (bufCur != bufEnd && decpt > 0) { *dest++ = *bufCur++; decpt--; }
    // Even if not in buffer.
    while (decpt > 0) { *dest++ = '0'; decpt--; }

    if ((fmt & STRING_FORMAT_ALTERNATE) != 0 || bufCur != bufEnd || precision > 0)
    {
      if (bufCur == out.result) *dest++ = '0';
      *dest++ = '.';
      while (decpt < 0 && precision > 0) { *dest++ = '0'; decpt++; precision--; }

      // Print rest of stuff.
      while (bufCur != bufEnd && precision > 0) { *dest++ = *bufCur++; precision--; }
      // And trailing zeros.
      while (precision > 0) { *dest++ = '0'; precision--; }
    }

    _modified(dest);
  }
  // Exponential form.
  else if (doubleForm == DF_EXPONENT)
  {
_ExponentialForm:
    StringUtil::dtoa(d, 2, precision + 1, &out);

    decpt = out.decpt;
    if (decpt == 9999) goto _InfOrNaN;

    // Reserve some space for number, we need +X.{PRECISION}e+123
    dest = beginManipulation(precision + 10, CONTAINER_OP_APPEND);
    if (FOG_IS_NULL(dest)) { err = ERR_RT_OUT_OF_MEMORY; goto _Ret; }

    bufCur = out.result;
    bufEnd = bufCur + out.length;

    *dest++ = *bufCur++;
    if ((fmt & STRING_FORMAT_ALTERNATE) != 0 || precision > 0)
    {
      if (bufCur != bufEnd || doubleForm == DF_EXPONENT) *dest++ = '.';
    }
    while (bufCur != bufEnd && precision > 0)
    {
      *dest++ = *bufCur++;
      precision--;
    }

    // Add trailing zeroes to fill out to ndigits unless this is
    // DF_SIGNIFICANT_DIGITS.
    if (doubleForm == DF_EXPONENT)
    {
      for (i = precision; i; i--) *dest++ = '0';
    }

    // Add the exponent.
    if (doubleForm == DF_EXPONENT || decpt > 1)
    {
      *dest++ = (ff.flags & STRING_FORMAT_CAPITALIZE_E_OR_X) ? 'E' : 'e';
      decpt--;
      if (decpt < 0)
        { *dest++ = '-'; decpt = -decpt; }
      else
        *dest++ = '+';

      dest = StringT_appendExponent(dest, decpt, '0');
    }

    _modified(dest);
  }
  // Significant digits form.
  else /* if (doubleForm == DF_SIGNIFICANT_DIGITS) */
  {
    char* save;
    if (d <= 0.0001 || d >= StringUtil::_mprec_log10(precision))
    {
      if (precision > 0) precision--;
      goto _ExponentialForm;
    }

    if (d < 1.0)
    {
      // What we want is ndigits after the point.
      StringUtil::dtoa(d, 3, precision, &out);
    }
    else
    {
      StringUtil::dtoa(d, 2, precision, &out);
    }

    decpt = out.decpt;
    if (decpt == 9999) goto _InfOrNaN;

    // Reserve some space for number.
    i = precision + 16;
    if (decpt > 0) i += (size_t)decpt;

    dest = beginManipulation(i, CONTAINER_OP_APPEND);
    if (FOG_IS_NULL(dest)) { err = ERR_RT_OUT_OF_MEMORY; goto _Ret; }

    save = dest;

    bufCur = out.result;
    bufEnd = bufCur + out.length;

    while (bufCur != bufEnd && decpt > 0) { *dest++ = *bufCur++; decpt--; precision--; }
    // Even if not in buffer.
    while (decpt > 0 && precision > 0) { *dest++ = '0'; decpt--; precision--; }

    if ((fmt & STRING_FORMAT_ALTERNATE) != 0 || bufCur != bufEnd)
    {
      if (dest == save) *dest++ = '0';
      *dest++ = '.';
      while (decpt < 0 && precision > 0) { *dest++ = '0'; decpt++; precision--; }

      // Print rest of stuff.
      while (bufCur != bufEnd && precision > 0){ *dest++ = *bufCur++; precision--; }
      // And trailing zeros.
      // while (precision > 0) { *dest++ = '0'; precision--; }
    }

    _modified(dest);
  }
  goto _Ret;

_InfOrNaN:
  err |= append(StubA((const char*)out.result, out.length));
_Ret:
  // Apply padding.
  numberLength = _d->length - beginLength;
  if (width != (size_t)-1 && width > numberLength)
  {
    size_t fill = width - numberLength;

    if ((fmt & STRING_FORMAT_LEFT) == 0)
    {
      if (savedPrecision == NO_PRECISION)
        err |= insert(beginLength + (sign != 0), '0', fill);
      else
        err |= insert(beginLength, ' ', fill);
    }
    else
    {
      err |= append(' ', fill);
    }
  }
  return err;
}

err_t StringW::appendDouble(double d, int doubleForm, const FormatParams& ff, const Locale* locale)
{
}
#endif

static const FormatReal StringT_formatRealDefult(DF_SIGNIFICANT_DIGITS, NO_FLAGS, NO_PRECISION, NO_WIDTH);

static err_t FOG_CDECL StringA_opDouble(StringA* self, uint32_t cntOp, double d)
{
  return fog_api.stringa_opDoubleEx(self, cntOp, d, &StringT_formatRealDefult);
}

static err_t FOG_CDECL StringW_opDouble(StringW* self, uint32_t cntOp, double d)
{
  return fog_api.stringw_opDoubleEx(self, cntOp, d, &StringT_formatRealDefult, NULL);
}

static err_t FOG_CDECL StringA_opDoubleEx(StringA* self, uint32_t cntOp, double d, const FormatReal* fmt)
{
  // TODO: StringA::opDoubleEx() missing implementation.
  return ERR_RT_NOT_IMPLEMENTED;
}

static err_t FOG_CDECL StringW_opDoubleEx(StringW* self, uint32_t cntOp, double d, const FormatReal* fmt, const Locale* locale)
{
  err_t err = ERR_OK;
  NTOAContext ctx;

  if (locale == NULL)
    locale = fog_api.locale_oPosix;

  if (cntOp == CONTAINER_OP_REPLACE)
    self->clear();

  uint32_t form = fmt->getForm();
  uint32_t flags = fmt->getFlags();

  size_t width = fmt->getWidth();
  size_t precision = fmt->getPrecision();
  size_t savedPrecision = precision;
  size_t initialLength = self->_d->length;

  CharW sign = CharW('\0');
  CharW zero = locale->getChar(LOCALE_CHAR_ZERO) - CharW('0');

  if (precision == NO_PRECISION)
    precision = 6;

  if (d < 0.0)
  {
    sign = locale->getChar(LOCALE_CHAR_MINUS);
    d = -d;
  }
  else if (flags & STRING_FORMAT_SIGN)
  {
    sign = locale->getChar(LOCALE_CHAR_PLUS);
  }
  else if (flags & STRING_FORMAT_BLANK)
  {
    sign = locale->getChar(LOCALE_CHAR_SPACE);
  }

  // --------------------------------------------------------------------------
  // [Form - Decimal]
  // --------------------------------------------------------------------------

  if (form == DF_DECIMAL)
  {
    StringUtil::dtoa(&ctx, d, 3, (int)precision);
    
    int32_t decpt = ctx.decpt;
    if (decpt == 9999)
      goto _InfOrNaN;

    uint8_t* bufCur = reinterpret_cast<uint8_t*>(ctx.result);
    uint8_t* bufEnd = bufCur + ctx.length;

    // Reserve some space for the number.
    size_t i = precision + 16;
    if (decpt > 0) i += (size_t)decpt;

    CharW* p = self->_add(i);
    if (FOG_IS_NULL(p))
      return ERR_RT_OUT_OF_MEMORY;

    while (bufCur != bufEnd && decpt > 0)
    {
      *p++ = zero + CharW(*bufCur++);
      decpt--;
    }

    // Even if not in buffer.
    while (decpt > 0)
    {
      *p++ = zero + CharW('0');
      decpt--;
    }

    if ((flags & STRING_FORMAT_ALTERNATE) != 0 || bufCur != bufEnd || precision > 0)
    {
      if (bufCur == reinterpret_cast<uint8_t*>(ctx.result))
        *p++ = zero + CharW('0');

      *p++ = locale->getChar(LOCALE_CHAR_DECIMAL_POINT);

      while (decpt < 0 && precision > 0)
      {
        *p++ = zero + CharW('0');
        decpt++;
        precision--;
      }

      // Print rest of stuff.
      while (*bufCur && precision > 0)
      {
        *p++ = zero + CharW(*bufCur++);
        precision--;
      }

      // And trailing zeros.
      while (precision > 0)
      {
        *p++ = zero + CharW('0');
        precision--;
      }
    }

    self->_modified(p);
  }

  // --------------------------------------------------------------------------
  // [Form - Exponential]
  // --------------------------------------------------------------------------

  else if (form == DF_EXPONENT)
  {
_ExponentialForm:
    StringUtil::dtoa(&ctx, d, 2, (int)precision + 1);

    int32_t decpt = ctx.decpt;
    if (decpt == 9999)
      goto _InfOrNaN;

    // Reserve some space for number, we need +X.{PRECISION}e+123
    CharW* p = self->_add(precision + 10);
    if (FOG_IS_NULL(p))
      return ERR_RT_OUT_OF_MEMORY;

    uint8_t* bufCur = reinterpret_cast<uint8_t*>(ctx.result);
    uint8_t* bufEnd = bufCur + ctx.length;

    *p++ = zero + CharW(*bufCur++);
    if ((flags & STRING_FORMAT_ALTERNATE) != 0 || precision > 0)
    {
      if (bufCur != bufEnd || form == DF_EXPONENT)
        *p++ = locale->getChar(LOCALE_CHAR_DECIMAL_POINT);
    }

    while (bufCur != bufEnd && precision > 0)
    {
      *p++ = zero + CharW(*bufCur++);
      precision--;
    }

    // Add trailing zeroes to fill out to ndigits unless this is
    // DF_SIGNIFICANT_DIGITS.
    if (form == DF_EXPONENT)
    {
      for (size_t i = precision; i; i--)
        *p++ = zero + CharW('0');
    }

    // Add the exponent.
    if (form == DF_EXPONENT || decpt > 1)
    {
      *p++ = locale->getChar(LOCALE_CHAR_EXPONENTIAL);

      if (--decpt < 0)
      {
        *p++ = locale->getChar(LOCALE_CHAR_MINUS);
        decpt = -decpt;
      }
      else
      {
        *p++ = locale->getChar(LOCALE_CHAR_PLUS);
      }

      p = StringT_appendExponent(p, (uint)decpt, zero + CharW('0'));
    }

    self->_modified(p);
  }

  // --------------------------------------------------------------------------
  // [Form - Significant Digits]
  // --------------------------------------------------------------------------

  else // if (form == DF_SIGNIFICANT_DIGITS)
  {
    CharW* save;

    if (d <= 0.0001 || d >= _mprec_log10((int)precision))
    {
      if (precision > 0)
        precision--;
      goto _ExponentialForm;
    }

    if (d < 1.0)
    {
      // What we want is ndigits after the point.
      StringUtil::dtoa(&ctx, d, 3, (int)++precision);
    }
    else
    {
      StringUtil::dtoa(&ctx, d, 2, (int)precision);
    }

    int32_t decpt = ctx.decpt;
    if (decpt == 9999)
      goto _InfOrNaN;

    // Reserve some space for number.
    size_t i = precision + 16;
    if (decpt > 0) i += (size_t)decpt;

    CharW* p = self->_add(i);
    if (FOG_IS_NULL(p))
      return ERR_RT_OUT_OF_MEMORY;

    save = p;

    uint8_t* bufCur = reinterpret_cast<uint8_t*>(ctx.result);
    uint8_t* bufEnd = bufCur + ctx.length;

    while (bufCur != bufEnd && decpt > 0)
    {
      *p++ = zero + CharW(*bufCur++);
      decpt--;
      precision--;
    }

    // Even if not in buffer.
    while (decpt > 0 && precision > 0)
    {
      *p++ = zero + CharW('0');
      decpt--;
      precision--;
    }

    if ((flags & STRING_FORMAT_ALTERNATE) != 0 || bufCur != bufEnd)
    {
      if (p == save) *p++ = zero + CharW('0');
      *p++ = locale->getChar(LOCALE_CHAR_DECIMAL_POINT);

      while (decpt < 0 && precision > 0)
      {
        *p++ = zero + CharW('0');
        decpt++;
        precision--;
      }

      // Print rest of stuff.
      while (bufCur != bufEnd && precision > 0)
      {
        *p++ = zero + CharW(*bufCur++);
        precision--;
      }

      // And trailing zeros.
      // while (precision > 0) { *p++ = zero + CharW('0'); precision--; }
    }

    self->_modified(p);
  }
  goto _Ret;

  // --------------------------------------------------------------------------
  // [Infinity or NaN]
  // --------------------------------------------------------------------------

_InfOrNaN:
  {
    CharW* p = self->_add(ctx.length + (!sign.isNull()));
    if (FOG_IS_NULL(p))
      return ERR_RT_OUT_OF_MEMORY;

    if (!sign.isNull())
      *p++ = sign;
    StringT_chcopy(p, (const char*)ctx.result, ctx.length);
  }

  // --------------------------------------------------------------------------
  // [Padding]
  // --------------------------------------------------------------------------

_Ret:
  {
    size_t numberLength = self->_d->length - initialLength;
    if (width != (size_t)-1 && width > numberLength)
    {
      size_t fill = width - numberLength;

      if ((flags & STRING_FORMAT_LEFT) == 0)
      {
        if (savedPrecision == NO_PRECISION)
          err |= self->insert(initialLength + !sign.isNull(), zero + CharW('0'), fill);
        else
          err |= self->insert(initialLength, CharW(' '), fill);
      }
      else
      {
        err |= self->append(CharW(' '), fill);
      }
    }
  }

  if (FOG_IS_ERROR(err))
    self->truncate(initialLength);

  return err;
}

// ============================================================================
// [Fog::String - OpVFormat]
// ============================================================================

// TODO: Errors are not propagated.

template<typename CharT>
static err_t FOG_CDECL StringT_opVFormatPrivate(CharT_(String)* self, uint32_t cntOp, const CharT* fmt, size_t fmtLength, const TextCodec* tc, const Locale* locale, va_list ap)
{
#define _FOG_VFORMAT_PARSE_NUMBER(_Out_)             \
  FOG_MACRO_BEGIN                                    \
    /* Clean-up. */                                  \
    _Out_ = 0;                                       \
                                                     \
    /* Remove zeros. */                              \
    while (c == CharT('0'))                          \
    {                                                \
      if (++fmt == fmtEnd)                           \
        goto _End;                                   \
      c = *fmt;                                      \
    }                                                \
                                                     \
    /* Parse number. */                              \
    while (CharT_Func::isAsciiDigit(c))              \
    {                                                \
      _Out_ = _Out_ * 10 + (CharT_Value)c - '0';     \
                                                     \
      if (++fmt == fmtEnd)                           \
        goto _End;                                   \
      c = *fmt;                                      \
    }                                                \
  FOG_MACRO_END

  // Choose the default text-codec is not provided.
  if (tc == NULL)
    tc = fog_api.textcodec_oCache[TEXT_CODEC_CACHE_ASCII];

  if (sizeof(CharT) > 1 && locale == NULL)
    locale = fog_api.locale_oPosix;

  if (cntOp == CONTAINER_OP_REPLACE)
    self->clear();

  if (fmtLength == 0)
    return ERR_OK;

  size_t initialLength = self->getLength();

  const CharT* fmtBeginChunk = fmt;
  const CharT* fmtEnd = fmt + fmtLength;

  CharT c;

  do {
    c = (CharT_(_Char)::Value)*fmt;

    if (c == CharT('%'))
    {
      if (fmtBeginChunk != fmt)
        self->append(CharT_(Stub)(fmtBeginChunk, (size_t)(fmt - fmtBeginChunk)));

      if (++fmt == fmtEnd)
        goto _End;

      uint32_t base = 10;
      uint32_t flags = NO_FLAGS;
      uint32_t size = 0;

      size_t width = NO_WIDTH;
      size_t precision = NO_PRECISION;

      bool isLongDouble = false;
      bool isLongInteger = false;

      // ----------------------------------------------------------------------
      // [Flags]
      // ----------------------------------------------------------------------

      for (;;)
      {
        c = (uint8_t)*fmt;

        if (c == CharT('#'))
          flags |= STRING_FORMAT_ALTERNATE;
        else if (c == CharT('0'))
          flags |= STRING_FORMAT_ZERO_PAD;
        else if (c == CharT('-'))
          flags |= STRING_FORMAT_LEFT;
        else if (c == CharT(' '))
          flags |= STRING_FORMAT_BLANK;
        else if (c == CharT('+'))
          flags |= STRING_FORMAT_SIGN;
        else if (c == CharT('\''))
          flags |= STRING_FORMAT_GROUP;
        else
          break;

        if (++fmt == fmtEnd)
          goto _End;
      }

      // ----------------------------------------------------------------------
      // [Width]
      // ----------------------------------------------------------------------

      if (CharT_Func::isAsciiDigit(c))
      {
        _FOG_VFORMAT_PARSE_NUMBER(width);
      }
      else if (c == CharT('*'))
      {
        int _width = va_arg(ap, int);
        if (_width < 0) _width = 0;
        if (_width > 4096) _width = 4096;
        width = (size_t)_width;

        if (++fmt == fmtEnd)
          goto _End;
        c = *fmt;
      }

      // ----------------------------------------------------------------------
      // [Precision]
      // ----------------------------------------------------------------------

      if (c == CharT('.'))
      {
        if (CharT_Func::isAsciiDigit(c))
        {
          _FOG_VFORMAT_PARSE_NUMBER(precision);
        }
        else if (c == '*')
        {
          int _precision = va_arg(ap, int);
          if (_precision < 0) _precision = 0;
          if (_precision > 4096) _precision = 4096;
          precision = (size_t)_precision;

          if (++fmt == fmtEnd)
            goto _End;
          c = *fmt;
        }
      }

      // ----------------------------------------------------------------------
      // [Argument Size]
      // ----------------------------------------------------------------------

      // TODO: 'j'  == sizeof(intmax_t).
      // TODO: 'll' == sizeof(long long).

      // 'h' and 'hh'.
      if (c == CharT('h'))
      {
        size = sizeof(short);

        if (++fmt == fmtEnd)
          goto _End;
        c = *fmt;

        if (c == CharT('h'))
        {
          size = sizeof(char);

          if (++fmt == fmtEnd)
            goto _End;
          c = *fmt;
        }
      }
      // 'j'.
      else if (c == 'j')
      {
        size = sizeof(uint64_t);

        if (++fmt == fmtEnd)
          goto _End;
        c = *fmt;
      }
      // 'l' and 'll'.
      else if (c == CharT('l'))
      {
        isLongInteger = true;
        size = sizeof(long);

        if (++fmt == fmtEnd)
          goto _End;
        c = *fmt;

        if (c == CharT('l'))
        {
          size = sizeof(uint64_t);

          if (++fmt == fmtEnd)
            goto _End;
          c = *fmt;
        }
      }
      // 'L'.
      else if (c == CharT('L'))
      {
        isLongDouble = true;

        if (++fmt == fmtEnd)
          goto _End;
        c = *fmt;
      }
      // 'q'.
      else if (c == 'q')
      {
        size = sizeof(uint64_t);

        if (++fmt == fmtEnd)
          goto _End;
        c = *fmt;
      }
      // 't'.
      else if (c == 't')
      {
        size = sizeof(ptrdiff_t);

        if (++fmt == fmtEnd)
          goto _End;
        c = *fmt;
      }
      // 'z'.
      else if (c == 'z' || c == 'Z')
      {
        size = sizeof(size_t);

        if (++fmt == fmtEnd)
          goto _End;
        c = *fmt;
      }

      // ----------------------------------------------------------------------
      // [Type]
      // ----------------------------------------------------------------------

      fmt++;
      switch (c)
      {
        // Signed integer.
        case 'd':
        case 'i':
        {
          int64_t i = (size >= sizeof(int64_t)) ? va_arg(ap, int64_t) : va_arg(ap, int);

          if (flags == NO_FLAGS && precision == NO_PRECISION && width == NO_WIDTH)
            self->appendInt(i);
          else
            self->appendInt(i, FormatInt(base, flags, precision, width));
          break;
        }

        // Unsigned integer.
        case 'o':
          base = 8;
          goto _FormatUnsigned;
        case 'X':
          flags |= STRING_FORMAT_CAPITALIZE;
        case 'x':
          base = 16;
        case 'u':
_FormatUnsigned:
        {
          uint64_t i = (size >= sizeof(uint64_t)) ? va_arg(ap, uint64_t) : va_arg(ap, uint);

          if (base == 10 && flags == NO_FLAGS && precision == NO_PRECISION && width == NO_WIDTH)
            self->appendInt(i);
          else
            self->appendInt(i, FormatInt(base, flags, precision, width));
          break;
        }

        // Float, double, and long double.
        case 'F':
        case 'E':
        case 'G':
          flags |= STRING_FORMAT_CAPITALIZE_E_OR_X;
        case 'f':
        case 'e':
        case 'g':
        {
          uint form;
          if (c == 'e' || c == 'E')
            form = DF_EXPONENT;
          else if (c == 'f' || c == 'F')
            form = DF_DECIMAL;
          else // if (c == 'g' || c == 'G')
            form = DF_SIGNIFICANT_DIGITS;

          double f = va_arg(ap, double);
          self->appendReal(f, FormatReal(form, flags, precision, width));
          break;
        }

        // Characters (Unicode or Latin1).
        case 'C':
          isLongInteger = true;
        case 'c':
        {
          if (precision == NO_PRECISION)
            precision = 1;
          if (width == NO_WIDTH)
            width = 0;

          size_t fill = (width > precision) ? width - precision : 0;

          if (fill && (flags & STRING_FORMAT_LEFT) == 0)
            self->append(CharT(' '), fill);

          self->append(CharT(va_arg(ap, uint)), precision);

          if (fill && (flags & STRING_FORMAT_LEFT) != 0)
            self->append(CharT(' '), fill);
          break;
        }

        // Strings.
        case 'S':
          isLongInteger = true;

        case 's':
          if (width == NO_WIDTH)
            width = 0;

          // Ansi-string.
          if (!isLongInteger)
          {
            const char* sData = va_arg(ap, const char*);
            size_t sLength = (precision != NO_PRECISION)
              ? (size_t)StringUtil::nlen(sData, precision)
              : (size_t)StringUtil::len(sData);

            if (sizeof(CharT) == 1)
            {
              StringA* selfA = reinterpret_cast<StringA*>(self);
              size_t fill = (width > sLength) ? width - sLength : 0;

              if (fill && (flags & STRING_FORMAT_LEFT) == 0) selfA->append(char(' '), fill);
              selfA->append(sData, sLength);
              if (fill && (flags & STRING_FORMAT_LEFT) != 0) selfA->append(char(' '), fill);
            }
            else
            {
              StringTmpW<128> tmp;
              tc->decode(tmp, StubA(sData, sLength));

              StringW* selfW = reinterpret_cast<StringW*>(self);
              size_t fill = (width > tmp.getLength()) ? width - tmp.getLength() : 0;

              if (fill && (flags & STRING_FORMAT_LEFT) == 0) selfW->append(CharW(' '), fill);
              selfW->append(tmp);
              if (fill && (flags & STRING_FORMAT_LEFT) != 0) selfW->append(CharW(' '), fill);
            }
          }
          // Wide-string.
          else
          {
            const CharW* sData = va_arg(ap, const CharW*);
            size_t sLength = (precision != NO_PRECISION)
              ? (size_t)StringUtil::nlen(sData, precision)
              : (size_t)StringUtil::len(sData);

            if (sizeof(CharT) == 1)
            {
              StringA* selfA = reinterpret_cast<StringA*>(self);
              size_t fill = (width > sLength) ? width - sLength : 0;

              if (fill && (flags & STRING_FORMAT_LEFT) == 0) selfA->append(' ', fill);
              tc->encode(*selfA, StubW(sData, sLength), NULL, NULL, CONTAINER_OP_APPEND);
              if (fill && (flags & STRING_FORMAT_LEFT) != 0) selfA->append(' ', fill);
            }
            else
            {
              StringW* selfW = reinterpret_cast<StringW*>(self);
              size_t fill = (width > sLength) ? width - sLength : 0;

              if (fill && (flags & STRING_FORMAT_LEFT) == 0) selfW->append(CharW(' '), fill);
              selfW->append(StubW(sData, sLength));
              if (fill && (flags & STRING_FORMAT_LEFT) != 0) selfW->append(CharW(' '), fill);
            }
          }
          break;

        // Pointer.
        case 'p':
          flags |= STRING_FORMAT_ALTERNATE;
          size = sizeof(void*);
          goto _FormatUnsigned;

        // Position receiver 'n'.
        case 'n':
        {
          void* p = va_arg(ap, void*);
          size_t n = self->getLength() - initialLength;

          if (size == sizeof(uint64_t))
            reinterpret_cast<uint64_t*>(p)[0] = (uint64_t)n;
          else if (size == sizeof(uint32_t))
            reinterpret_cast<uint32_t*>(p)[0] = (uint32_t)size;
          else if (size == sizeof(uint16_t))
            reinterpret_cast<uint16_t*>(p)[0] = (uint16_t)size;
          else if (size == sizeof(uint8_t ))
            reinterpret_cast<uint8_t*>(p)[0] = (uint8_t)size;
          else // if (size == 0)
            reinterpret_cast<uint*>(p)[0] = (uint)size;

          break;
        }

        // Percent.
        case '%':
          // skip one "%" if its legal "%%", otherwise send everything
          // to the output.
          if (fmtBeginChunk + 1 == fmt)
            fmtBeginChunk++;
          goto _Continue;

        // Unsupported or end of input.
        default:
          goto _Continue;
      }

      fmtBeginChunk = fmt;
    }
    else
    {
      fmt++;
    }

_Continue:
    ;
  } while (fmt != fmtEnd);

_End:
  if (fmtBeginChunk != fmt)
    self->append(CharT_(Stub)(fmtBeginChunk, (size_t)(fmt - fmtBeginChunk)));
  return ERR_OK;

#undef _FOG_VFORMAT_PARSE_NUMBER
}

static err_t FOG_CDECL StringA_opVFormatStubA(StringA* self, uint32_t cntOp, const StubA* fmt, const TextCodec* tc, va_list ap)
{
  return StringT_opVFormatPrivate<char>(self, cntOp, fmt->getData(), fmt->getComputedLength(), tc, NULL, ap);
}

static err_t FOG_CDECL StringA_opVFormatStringA(StringA* self, uint32_t cntOp, const StringA* fmt, const TextCodec* tc, va_list ap)
{
  StringA fmtCopy(*fmt);
  return StringT_opVFormatPrivate<char>(self, cntOp, fmtCopy.getData(), fmtCopy.getLength(), tc, NULL, ap);
}

static err_t FOG_CDECL StringW_opVFormatStubA(StringW* self, uint32_t cntOp, const StubA* fmt, const TextCodec* tc, const Locale* locale, va_list ap)
{
  StringTmpW<256> fmtW;

  if (tc == NULL)
    tc = fog_api.textcodec_oCache[TEXT_CODEC_CACHE_ASCII];
  FOG_RETURN_ON_ERROR(tc->decode(fmtW, *fmt));

  return StringT_opVFormatPrivate<CharW>(self, cntOp, fmtW.getData(), fmtW.getLength(), tc, locale, ap);
}

static err_t FOG_CDECL StringW_opVFormatStubW(StringW* self, uint32_t cntOp, const StubW* fmt, const TextCodec* tc, const Locale* locale, va_list ap)
{
  return StringT_opVFormatPrivate<CharW>(self, cntOp, fmt->getData(), fmt->getComputedLength(), tc, locale, ap);
}

static err_t FOG_CDECL StringW_opVFormatStringW(StringW* self, uint32_t cntOp, const StringW* fmt, const TextCodec* tc, const Locale* locale, va_list ap)
{
  StringW fmtCopy(*fmt);
  return StringT_opVFormatPrivate<CharW>(self, cntOp, fmtCopy.getData(), fmtCopy.getLength(), tc, locale, ap);
}

// ============================================================================
// [Fog::String - OpZFormat]
// ============================================================================

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_opZFormatStub(CharT_(String)* self, uint32_t cntOp, const SrcT_(Stub)* fmt, CharT_Type lex, const CharT_(String)* args, size_t argsLength)
{
  if (cntOp == CONTAINER_OP_REPLACE)
    self->clear();

  err_t err = ERR_OK;

  const SrcT* fmtBeg = fmt->getData();
  const SrcT* fmtCur = fmtBeg;
  const SrcT* fmtEnd = fmtBeg + fmt->getComputedLength();

  while (fmtCur != fmtEnd)
  {
    if (*fmtCur == lex)
    {
      fmtBeg = fmtCur;

      err = self->_append(CharT_(Stub)(fmtBeg, (size_t)(fmtCur - fmtBeg)));
      if (FOG_IS_ERROR(err))
        goto _End;

      if (++fmtCur != fmtEnd)
      {
        SrcT ch = *fmtCur;

        if (ch >= SrcT('0') && ch <= SrcT('9'))
        {
          size_t n = (size_t)(uint8_t)ch - (uint32_t)'0';
          if (n < argsLength)
          {
            err = self->append(args[n]);
            if (FOG_IS_ERROR(err))
              goto _End;

            fmtBeg = fmtCur + 1;
          }
        }
        else if (ch == lex)
          fmtBeg++;
      }
      else
        break;
    }

    fmtCur++;
  }

  if (fmtCur != fmtBeg)
    err = self->append(CharT_(Stub)(fmtBeg, (size_t)(fmtCur - fmtBeg)));

_End:
  return err;
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_opZFormatString(CharT_(String)* self, uint32_t cntOp, const SrcT_(String)* fmt, CharT_Type lex, const CharT_(String)* args, size_t argsLength)
{
  SrcT_(String) fmtCopy(*fmt);
  SrcT_(Stub) fmtStub(fmtCopy.getData(), fmtCopy.getLength());

  return StringT_opZFormatStub<CharT, SrcT>(self, cntOp, &fmtStub, lex, args, argsLength);
}

// ============================================================================
// [Fog::String - OpNormalizeSlashes]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_opNormalizeSlashes(CharT_(String)* self, uint32_t cntOp, const CharT_(String)* other, const Range* range, uint32_t slashForm)
{
  if (slashForm >= SLASH_FORM_COUNT)
    return ERR_RT_INVALID_ARGUMENT;

  CharT from = CharT('\\');
  CharT to = CharT('/');

  if (slashForm == SLASH_FORM_BACKWARD)
    swap<CharT>(from, to);

  size_t oLength = other->getLength();
  size_t oStart, oEnd;

  if (!Range::fit(oStart, oEnd, oLength, range))
  {
    if (cntOp == CONTAINER_OP_REPLACE)
      self->clear();
    return ERR_OK;
  }

  if (cntOp == CONTAINER_OP_REPLACE && self == other)
  {
    if (oStart != 0 || oEnd != oLength)
      FOG_RETURN_ON_ERROR(self->slice(Range(oStart, oEnd)));
    return self->replace(from, to);
  }

  oLength = oEnd - oStart;

  CharT* p = self->_prepare(cntOp, oLength);
  const CharT* s = other->getData() + oStart;

  if (FOG_IS_NULL(p))
    return ERR_RT_OUT_OF_MEMORY;

  for (size_t i = 0; i < oLength; i++)
  {
    CharT c = s[i];
    if (c == from)
      c = to;
    p[i] = c;
  }

  return ERR_OK;
}

// ============================================================================
// [Fog::String - Prepend]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_prependChars(CharT_(String)* self, CharT_Type ch, size_t length)
{
  return self->insert(0, CharT(ch), length);
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_prependStub(CharT_(String)* self, const SrcT_(Stub)* stub)
{
  return self->insert(0, *stub);
}

static err_t FOG_CDECL StringW_prependStubCodec(StringW* self, const StubA* other, const TextCodec* tc)
{
  return self->insert(0, *other, *tc);
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_prependString(CharT_(String)* self, const SrcT_(String)* other)
{
  return self->insert(0, *other);
}

static err_t FOG_CDECL StringW_prependStringCodec(StringW* self, const StringA* other, const TextCodec* tc)
{
  return self->insert(0, *other, *tc);
}

// ============================================================================
// [Fog::String - Insert]
// ============================================================================

template<typename CharT>
static CharT* StringT_prepareInsert(CharT_(String)* self, size_t index, size_t length)
{
  FOG_ASSERT(length > 0);

  CharT_(StringData)* d = self->_d;
  size_t before = d->length;

  if (!Math::canSum(before, length))
    return NULL;

  size_t after = before + length;
  size_t moveBy;

  if (index > before)
    index = before;
  moveBy = before - index;

  if (d->reference.get() > 1 || d->capacity < after)
  {
    size_t optimalCapacity = ContainerUtil::getGrowCapacity(
      sizeof(CharT_(StringData)), sizeof(CharT), before, after);

    d = CharI_(String)::_dCreate(optimalCapacity, CharT_(Stub)(d->data, index));
    if (FOG_IS_NULL(d))
      return NULL;

    StringT_chcopy(d->data + index + length, self->_d->data + index, moveBy);
    atomicPtrXchg(&self->_d, d)->release();
  }
  else
  {
    StringT_chmove(d->data + index + length, d->data + index, moveBy);
  }

  d->hashCode = 0;
  d->length = after;
  d->data[after] = 0;
  return d->data + index;
}

template<typename CharT>
static err_t FOG_CDECL StringT_insertChars(CharT_(String)* self, size_t index, CharT_Type ch, size_t length)
{
  if (length == DETECT_LENGTH)
    return ERR_RT_INVALID_ARGUMENT;

  if (length == 0)
    return ERR_OK;

  CharT* p = StringT_prepareInsert<CharT>(self, index, length);
  if (FOG_IS_NULL(p))
    return ERR_RT_OUT_OF_MEMORY;

  StringT_chfill(p, ch, length);
  return ERR_OK;
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_insertStub(CharT_(String)* self, size_t index, const SrcT_(Stub)* stub)
{
  const SrcT* sData = stub->getData();
  size_t sLength = stub->getComputedLength();

  if (sLength == 0)
    return ERR_OK;

  CharT* p = StringT_prepareInsert<CharT>(self, index, sLength);
  if (FOG_IS_NULL(p))
    return ERR_RT_OUT_OF_MEMORY;

  StringT_chcopy(p, sData, sLength);
  return ERR_OK;
}

static err_t FOG_CDECL StringW_insertStubCodec(StringW* self, size_t index, const StubA* other, const TextCodec* tc)
{
  StringTmpW<256> tmp;
  FOG_RETURN_ON_ERROR(tc->decode(tmp, *other));

  return self->insert(index, tmp);
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_insertString(CharT_(String)* self, size_t index, const SrcT_(String)* other)
{
  const SrcT* sData = other->getData();
  size_t sLength = other->getLength();

  if (sLength == 0)
    return ERR_OK;

  CharT* p = StringT_prepareInsert<CharT>(self, index, sLength);
  if (FOG_IS_NULL(p))
    return ERR_RT_OUT_OF_MEMORY;

  StringT_chcopy(p, sData, sLength);
  return ERR_OK;
}

static err_t FOG_CDECL StringW_insertStringCodec(StringW* self, size_t index, const StringA* other, const TextCodec* tc)
{
  StringTmpW<256> tmp;
  FOG_RETURN_ON_ERROR(tc->decode(tmp, *other));

  return self->insert(index, tmp);
}

// ============================================================================
// [Fog::String - Remove]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_removeRange(CharT_(String)* self, const Range* range)
{
  CharT_(StringData)* d = self->_d;
  size_t dLength = d->length;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, dLength, range))
    return ERR_OK;

  size_t lenPart1 = rStart;
  size_t lenPart2 = dLength - rEnd;
  size_t lenAfter = lenPart1 + lenPart2;

  if (d->reference.get() > 1)
  {
    CharT_(StringData)* newd = CharI_(String)::_dCreate(lenAfter);
    if (FOG_IS_NULL(newd))
      return ERR_RT_OUT_OF_MEMORY;

    StringT_chcopy(newd->data, d->data, lenPart1);
    StringT_chcopy(newd->data + rStart, d->data + rEnd, lenPart2);

    newd->length = lenAfter;
    newd->data[lenAfter] = 0;

    atomicPtrXchg(&self->_d, newd)->release();
    return ERR_OK;
  }
  else
  {
    StringT_chcopy(d->data + rStart, d->data + rEnd, lenPart2);

    d->hashCode = 0;
    d->length = lenAfter;
    d->data[lenAfter] = 0;
    return ERR_OK;
  }
}

template<typename CharT>
static err_t FOG_CDECL StringT_removeRangeList(CharT_(String)* self, const Range* range, size_t rangeLength)
{
  if (range == NULL || rangeLength == 0)
    return ERR_OK;

  CharT_(StringData)* d = self->_d;

  size_t i;
  size_t dLength = d->length;

  if (range[rangeLength-1].getEnd() > dLength)
    return ERR_RT_INVALID_ARGUMENT;

  if (d->reference.get() > 1)
  {
    size_t deleteLength = 0;
    size_t lengthAfter;

    // Get the count of characters which will be removed from the string so
    // we can calculate the count of characters after removal. We also need
    // to validate whether the 'range' list do not overlap and its sorted.
    size_t last = 0;
    for (i = 0; i < rangeLength; i++)
    {
      if (!range[i].isValid() || range[i].getStart() < last)
        return ERR_RT_INVALID_ARGUMENT;

      deleteLength += range[i].getLengthNoCheck();
      last = range[i].getEnd();
    }

    FOG_ASSERT(deleteLength <= dLength);
    lengthAfter = dLength - deleteLength;

    if (lengthAfter == 0)
    {
      self->clear();
      return ERR_OK;
    }

    CharT_(StringData)* newd = CharI_(String)::_dCreate(lengthAfter);
    if (FOG_IS_NULL(newd))
      return ERR_RT_OUT_OF_MEMORY;

    CharT* dData = newd->data;
    const CharT* sData = d->data;

    size_t dPos = range[0].getStart();
    size_t sPos = dPos;

    StringUtil::copy(dData, sData, dPos);

    i = 0;
    do {
      FOG_ASSERT(range[i].isValid());

      sPos += range[i].getLengthNoCheck();
      size_t j = ((++i == rangeLength) ? dLength : range[i].getStart()) - sPos;

      StringT_chcopy(dData + dPos, sData + sPos, j);
      dPos += j;
      sPos += j;
    } while (i != rangeLength);

    newd->length = lengthAfter;
    newd->data[lengthAfter] = 0;

    atomicPtrXchg(&self->_d, newd)->release();
    return ERR_OK;
  }
  else
  {
    CharT* data = d->data;

    size_t dPos = range[0].getStart();
    size_t sPos = dPos;

    // Validate first.
    size_t last = 0;
    for (i = 0; i < rangeLength; i++)
    {
      if (!range[i].isValid() || range[i].getStart() < last)
        return ERR_RT_INVALID_ARGUMENT;
      last = range[i].getEnd();
    }

    i = 0;
    do {
      FOG_ASSERT(range[i].isValid());

      sPos += range[i].getLengthNoCheck();
      size_t j = ((++i == rangeLength) ? dLength : range[i].getStart()) - sPos;

      StringT_chcopy(data + dPos, data + sPos, j);
      dPos += j;
      sPos += j;
    } while (i != rangeLength);

    d->hashCode = 0;
    d->length = dPos;
    data[dPos] = 0;

    return ERR_OK;
  }
}

template<typename CharT>
static err_t FOG_CDECL StringT_removeChar(CharT_(String)* self, const Range* range, CharT_Type ch, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;
  size_t dLength = d->length;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, dLength, range))
    return ERR_OK;

  CharT* sBegin = d->data;
  CharT* sCur = sBegin + rStart;
  CharT* sEnd = sBegin + rEnd;
  CharT* dCur;

  CharT_Value ciArray[4];
  uint ciCount;

  if (cs == CASE_SENSITIVE || (ciCount = StringT_getCaseInsensitiveChars<CharT_Value, CharT>(ciArray, CharT(ch))) == 1)
  {
    for (;;)
    {
      if (*sCur == ch)
        break;
      if (++sCur == sEnd)
        return ERR_OK;
    }

    if (d->reference.get() > 1)
    {
      rStart = sCur - sBegin;
      FOG_RETURN_ON_ERROR(self->_detach());

      d = self->_d;
      sBegin = d->data;

      sCur = sBegin + rStart;
      sEnd = sBegin + rEnd;
    }
    dCur = sCur;

    do {
      if (*sCur != ch)
        *dCur++ = *sCur;
    } while (++sCur != sEnd);
  }
  else if (ciCount == 2)
  {
    for (;;)
    {
      if (*sCur == ciArray[0] || *sCur == ciArray[0])
        break;
      if (++sCur == sEnd)
        return ERR_OK;
    }

    if (d->reference.get() > 1)
    {
      rStart = sCur - sBegin;
      FOG_RETURN_ON_ERROR(self->_detach());

      d = self->_d;
      sBegin = d->data;

      sCur = sBegin + rStart;
      sEnd = sBegin + rEnd;
    }

    dCur = sCur;
    do {
      if (*sCur != ciArray[0] && *sCur != ciArray[1])
        *dCur++ = *sCur;
    } while (++sCur != sEnd);
  }
  else
  {
    for (;;)
    {
      if (*sCur == ciArray[0] || *sCur == ciArray[1] || *sCur == ciArray[2])
        break;
      if (++sCur == sEnd)
        return ERR_OK;
    }

    if (d->reference.get() > 1)
    {
      rStart = sCur - sBegin;
      FOG_RETURN_ON_ERROR(self->_detach());

      d = self->_d;
      sBegin = d->data;

      sCur = sBegin + rStart;
      sEnd = sBegin + rEnd;
    }

    dCur = sCur;
    do {
      if (*sCur != ciArray[0] || *sCur != ciArray[1] || *sCur != ciArray[2])
        *dCur++ = *sCur;
    } while (++sCur != sEnd);
  }

  size_t tail = dLength - rEnd;
  StringT_chcopy(dCur, sCur, tail);

  size_t after = (size_t)(dCur - d->data) + tail;
  d->hashCode = 0;
  d->length = after;
  d->data[after] = 0;
  return ERR_OK;
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_removeHelper(CharT_(String)* self, const Range& range, const SrcT* bData, size_t bLength, uint32_t cs)
{
  FOG_ASSERT(bLength > 1);
  FOG_ASSERT(cs < CASE_SENSITIVITY_COUNT);

  CharT_(StringData)* d = self->_d;

  size_t rStart = range.getStart();
  size_t rEnd = range.getEnd();

  CharT* aData = d->data;
  size_t aLength = d->length;

  // Get the first index of the pattern 'b' in the string 'a'. If the pattern
  // 'b' is not inside the string 'a' then we will stop here to prevent memory
  // allocation / deallocation.
  size_t i = StringUtil::indexOf(aData + rStart, rEnd - rStart, bData, bLength, cs);
  if (i == INVALID_INDEX)
    return ERR_OK;

  i += rStart;

  if (d->reference.get() > 1)
  {
    CharT_(StringData)* newd = CharI_(String)::_dCreate(aLength - bLength);
    if (FOG_IS_NULL(newd))
      return ERR_RT_OUT_OF_MEMORY;

    CharT* p = newd->data;
    rStart = 0;

    do {
      size_t copyLength = i - rStart;
      StringT_chcopy(p, aData + rStart, copyLength);

      p += copyLength;
      rStart = i + bLength;

      i = StringUtil::indexOf(aData + rStart, rEnd - rStart, bData, bLength, cs);
    } while (i != INVALID_INDEX);

    if (rEnd != aLength)
    {
      size_t copyLength = aLength - rEnd;
      StringT_chcopy(p, aData + rEnd, copyLength);

      p += copyLength;
    }

    aLength = (size_t)(p - newd->data);

    newd->hashCode = 0;
    newd->length = aLength;
    newd->data[aLength] = 0;

    atomicPtrXchg(&self->_d, newd)->release();
    return ERR_OK;
  }
  else
  {
    CharT* p = aData + i;
    rStart = i;

    while ((i = StringUtil::indexOf(aData + rStart, rEnd - rStart, bData, bLength, cs)) != INVALID_INDEX)
    {
      size_t copyLength = i - rStart;
      StringT_chcopy(p, aData + rStart, copyLength);

      p += copyLength;
      rStart = i + bLength;
    }

    if (rEnd != aLength)
    {
      size_t copyLength = aLength - rEnd;
      StringT_chcopy(p, aData + rEnd, copyLength);

      p += copyLength;
    }

    aLength = (size_t)(p - aData);

    d->hashCode = 0;
    d->length = aLength;
    d->data[aLength] = 0;

    return ERR_OK;
  }
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_removeStub(CharT_(String)* self, const Range* range, const SrcT_(Stub)* stub, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;

  size_t dLength = d->length;
  size_t rStart, rEnd;

  if (!Range::fit(rStart, rEnd, dLength, range))
    return ERR_OK;

  size_t bLength = stub->getComputedLength();
  if (bLength == 0 || bLength > rEnd - rStart)
    return ERR_OK;

  if (bLength == 1)
    return self->remove(Range(rStart, rEnd), CharT(stub->getData()[0]), cs);
  else
    return StringT_removeHelper<CharT, SrcT>(self, Range(rStart, rEnd), stub->getData(), bLength, cs);
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_removeString(CharT_(String)* self, const Range* range, const SrcT_(String)* other, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;
  SrcT_(StringData)* other_d = other->_d;

  size_t dLength = d->length;
  size_t rStart, rEnd;

  if (!Range::fit(rStart, rEnd, dLength, range))
    return ERR_OK;

  size_t bLength = other->getLength();
  if (bLength == 0 || bLength > rEnd - rStart)
    return ERR_OK;

  if (bLength == 1)
    return self->remove(Range(rStart, rEnd), other_d->data[0], cs);

  // Special case. If the strings are the same and we passed the range check
  // condition then the result is empty string, ignoring case-sensitivity,
  // because both strings are equal.
  if (sizeof(CharT) == sizeof(SrcT) && (void*)d == (void*)other_d)
  {
    self->clear();
    return ERR_OK;
  }

  return StringT_removeHelper<CharT, SrcT>(self, Range(rStart, rEnd), other_d->data, bLength, cs);
}

template<typename CharT>
static err_t FOG_CDECL StringT_removeRegExp(CharT_(String)* self, const Range* range, const CharT_(RegExp)* re)
{
  CharT_(StringData)* d = self->_d;
  size_t dLength = d->length;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, dLength, range))
    return ERR_OK;

  const CharT* dData = d->data;
  List<Range> matches;

  for (;;)
  {
    Range r = re->indexIn(dData, dLength, Range(rStart, rEnd));
    if (r.getStart() == INVALID_INDEX)
      break;

    matches.append(r);
    rStart = r.getEnd();
  }

  return self->remove(matches.getData(), matches.getLength());
}

// ============================================================================
// [Fog::String - Replace]
// ============================================================================

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_replaceRangeStub(CharT_(String)* self, const Range* range, const SrcT_(Stub)* replacement)
{
  CharT_(StringData)* d = self->_d;

  size_t dLength = d->length;
  size_t rStart, rEnd;

  if (!Range::fit(rStart, rEnd, dLength, range))
    return ERR_OK;

  const CharT* replacementData = replacement->getData();
  size_t replacementLength = replacement->getComputedLength();

  if (d->reference.get() == 1)
  {
    size_t after = dLength - (rEnd - rStart);
    if (after > SIZE_MAX - replacementLength)
      return ERR_RT_OUT_OF_MEMORY;
    after += replacementLength;

    if (d->capacity >= after)
    {
      CharT* dData = d->data;
      CharT* dStart = dData + rStart;

      StringT_chmove(dStart + replacementLength, dData + rEnd, dLength - rEnd);
      StringT_chcopy(dStart, replacementData, replacementLength);

      d->hashCode = 0;
      d->length = after;
      d->data[after] = 0;
      return ERR_OK;
    }
  }

  Range r(rStart, rEnd);
  return self->replace(&r, 1, CharT_(Stub)(replacementData, replacementLength));
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_replaceRangeString(CharT_(String)* self, const Range* range, const SrcT_(String)* replacement)
{
  if (sizeof(CharT) == sizeof(SrcT) && (void*)self == (void*)replacement)
  {
    SrcT_(String) replacementCopy(*replacement);
    SrcT_(Stub) replacementStub(replacementCopy.getData(), replacementCopy.getLength());

    return StringT_replaceRangeStub<CharT, SrcT>(self, range, &replacementStub);
  }
  else
  {
    SrcT_(Stub) replacementStub(replacement->getData(), replacement->getLength());

    return StringT_replaceRangeStub<CharT, SrcT>(self, range, &replacementStub);
  }
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_replaceRangeListStub(CharT_(String)* self, const Range* range, size_t rangeLength, const SrcT_(Stub)* replacement)
{
  size_t i;

  const CharT* sData = self->getData();
  size_t sLength = self->getLength();

  const SrcT* replacementData = replacement->getData();
  size_t replacementLength = replacement->getComputedLength();

  size_t finalLength;
  size_t rLast;

  // Get total count of characters we remove, and validate the range[] array.
  {
    size_t rTotal = 0;

    for (i = 0, rLast = 0; i < rangeLength; i++)
    {
      size_t rStart = range[i].getStart();
      size_t rEnd = range[i].getEnd();

      if (rStart < rLast || rStart >= rEnd)
        return ERR_RT_INVALID_ARGUMENT;

      rTotal += rEnd - rStart;
      rLast = rEnd;
    }

    if (rLast >= sLength)
      return ERR_RT_INVALID_ARGUMENT;

    // Get total count of characters we add.
    size_t addLength = replacementLength * rangeLength;
    if (addLength / rangeLength != replacementLength)
      return ERR_RT_OUT_OF_MEMORY;

    // Get final length (after all areas replaced).
    finalLength = sLength - rTotal;
    if (!Math::canSum(finalLength, addLength))
      return ERR_RT_OUT_OF_MEMORY;

    finalLength += addLength;
  }

  CharT_(StringData)* newd = CharI_(String)::_dCreate(finalLength);
  if (FOG_IS_NULL(newd))
    return ERR_RT_OUT_OF_MEMORY;

  CharT* p = newd->data;
  size_t t;

  for (i = 0, rLast = 0; i < rangeLength; i++)
  {
    size_t rStart = range[i].getStart();
    size_t rEnd = range[i].getEnd();

    // Begin.
    t = rStart - rLast;

    StringT_chcopy(p, sData + rLast, t);
    p += t;

    StringT_chcopy(p, replacementData, replacementLength);
    p += replacementLength;

    rLast = rEnd;
  }

  // Trailing area.
  t = sLength - rLast;

  StringT_chcopy(p, sData + rLast, t);
  p += t;

  // Be sure that final length was correctly calculated.
  FOG_ASSERT(p == newd->data + finalLength);

  newd->length = finalLength;
  newd->data[finalLength] = 0;

  atomicPtrXchg(&self->_d, newd)->release();
  return ERR_OK;
}

template<typename CharT, typename SrcT>
static err_t FOG_CDECL StringT_replaceRangeListString(CharT_(String)* self, const Range* range, size_t rangeLength, const SrcT_(String)* replacement)
{
  SrcT_(Stub) replacementStub(replacement->getData(), replacement->getLength());
  return StringT_replaceRangeListStub<CharT, SrcT>(self, range, rangeLength, &replacementStub);
}

template<typename CharT>
static err_t FOG_CDECL StringT_replaceChar(CharT_(String)* self, const Range* range, CharT_Type before, CharT_Type after, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;

  size_t dLength = d->length;
  size_t rStart, rEnd;

  if (!Range::fit(rStart, rEnd, dLength, range))
    return ERR_OK;

  CharT* sCur = d->data + rStart;
  CharT* sEnd = d->data + rEnd;

  CharT_Value ciArray[4];
  uint ciCount;

  if (cs == CASE_SENSITIVE || (ciCount = StringT_getCaseInsensitiveChars<CharT_Value, CharT>(ciArray, CharT(before))) == 1)
  {
    for (;;)
    {
      if (*sCur == before)
        break;
      if (++sCur == sEnd)
        return ERR_OK;
    }

    if (d->reference.get() > 1)
    {
      rStart = (size_t)(sCur - d->data);

      FOG_RETURN_ON_ERROR(self->_detach());
      d = self->_d;

      sCur = d->data + rStart;
      sEnd = d->data + rEnd;
    }

    do {
      if (*sCur == before)
        *sCur = after;
    } while (++sCur != sEnd);
  }
  else if (ciCount == 2)
  {
    for (;;)
    {
      if (*sCur == ciArray[0] || *sCur == ciArray[1])
        break;
      if (++sCur == sEnd)
        return ERR_OK;
    }

    if (d->reference.get() > 1)
    {
      rStart = (size_t)(sCur - d->data);

      FOG_RETURN_ON_ERROR(self->_detach());
      d = self->_d;

      sCur = d->data + rStart;
      sEnd = d->data + rEnd;
    }

    do {
      if (*sCur == ciArray[0] || *sCur == ciArray[1])
        *sCur = after;
    } while (++sCur != sEnd);
  }
  else
  {
    for (;;)
    {
      if (*sCur == ciArray[0] || *sCur == ciArray[1] || *sCur == ciArray[2])
        break;
      if (++sCur == sEnd)
        return ERR_OK;
    }

    if (d->reference.get() > 1)
    {
      rStart = (size_t)(sCur - d->data);

      FOG_RETURN_ON_ERROR(self->_detach());
      d = self->_d;

      sCur = d->data + rStart;
      sEnd = d->data + rEnd;
    }

    do {
      if (*sCur == ciArray[0] || *sCur == ciArray[1] || *sCur == ciArray[2])
        *sCur = after;
    } while (++sCur != sEnd);
  }

  d->hashCode = 0;
  return ERR_OK;
}

template<typename CharT>
static err_t FOG_CDECL StringT_replaceString(CharT_(String)* self, const Range* range, const CharT_(String)* pattern, const CharT_(String)* replacement, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;
  size_t dLength = d->length;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, dLength, range))
    return ERR_OK;

  if (cs >= CASE_SENSITIVITY_COUNT)
    cs = CASE_INSENSITIVE;

  CharT* dData = d->data;

  // Check if there is some match to prevent early allocation / deallocation.
  rStart = StringUtil::indexOf(dData + rStart, rEnd - rStart, pattern->getData(), pattern->getLength(), cs);
  if (rStart == INVALID_INDEX)
    return ERR_OK;

  // Create a weak copy of pattern and replacement to prevent memory corruption
  // in case that the pattern or replacement are the same of 'self'.
  CharT_(String) patternCopy(*pattern);
  CharT_(String) replacementCopy(*replacement);

  const CharT* pData = patternCopy.getData();
  size_t pLength = patternCopy.getLength();

  if (pLength == 0)
    return ERR_OK;

  size_t rLength = replacementCopy.getLength();

  if (d->reference.get() == 1 && pLength == rLength)
  {
    // Special case. If the pattern length is the same as the replacement
    // length and we have non-shared data instance then we can use faster
    // algorithm to replace the characters in the destination.
    do {
      rEnd = rStart + pLength;
      StringT_chcopy(dData + rStart, pData, pLength);

      rStart = StringUtil::indexOf(dData + rStart, rEnd - rStart, pData, pLength, cs);
    } while (rStart != INVALID_INDEX);

    return ERR_OK;
  }
  else
  {
    // Common case. Collect all matches and then replace them by one run.
    List<Range> matches;

    do {
      rEnd = rStart + pLength;
      matches.append(Range(rStart, rEnd));

      rStart = StringUtil::indexOf(dData + rStart, rEnd - rStart, pData, pLength, cs);
    } while (rStart != INVALID_INDEX);

    return self->replace(matches.getData(), matches.getLength(), replacementCopy);
  }
}

template<typename CharT>
static err_t FOG_CDECL StringT_replaceRegExp(CharT_(String)* self, const Range* range, const CharT_(RegExp)* re, const CharT_(String)* replacement)
{
  CharT_(StringData)* d = self->_d;
  size_t dLength = d->length;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, dLength, range))
    return ERR_OK;

  const CharT* dData = d->data;
  List<Range> matches;

  for (;;)
  {
    Range r = re->indexIn(dData, dLength, Range(rStart, rEnd));
    if (r.getStart() == INVALID_INDEX)
      break;

    matches.append(r);
    rStart = r.getEnd();
  }

  if (matches.getLength() == 0)
    return ERR_OK;
  else
    return self->replace(matches.getData(), matches.getLength(), *replacement);
}

// ============================================================================
// [Fog::String - Lower / Upper]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_lower(CharT_(String)* self, const Range* range)
{
  CharT_(StringData)* d = self->_d;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return ERR_OK;

  CharT* p = d->data + rStart;
  CharT* pEnd = d->data + rEnd;

  do {
    CharT c = *p;
    if (CharT_Func::toLower(c) != c)
      goto _Modify;
  } while (++p != pEnd);

  return ERR_OK;

_Modify:
  if (d->reference.get() > 1)
  {
    rStart = (size_t)(p - d->data);

    FOG_RETURN_ON_ERROR(self->_detach());
    d = self->_d;

    p = d->data + rStart;
    pEnd = d->data + rEnd;
  }

  do {
    *p = CharT_Func::toLower(*p);
  } while (++p != pEnd);

  d->hashCode = 0;
  return ERR_OK;
}

template<typename CharT>
static err_t FOG_CDECL StringT_upper(CharT_(String)* self, const Range* range)
{
  CharT_(StringData)* d = self->_d;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return ERR_OK;

  CharT* p = d->data + rStart;
  CharT* pEnd = d->data + rEnd;

  do {
    CharT c = *p;
    if (CharT_Func::toUpper(c) != c)
      goto _Modify;
  } while (++p != pEnd);

  return ERR_OK;

_Modify:
  if (d->reference.get() > 1)
  {
    rStart = (size_t)(p - d->data);

    FOG_RETURN_ON_ERROR(self->_detach());
    d = self->_d;

    p = d->data + rStart;
    pEnd = d->data + rEnd;
  }

  do {
    *p = CharT_Func::toUpper(*p);
  } while (++p != pEnd);

  d->hashCode = 0;
  return ERR_OK;
}

// ============================================================================
// [Fog::String - Trim / Simplify]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_trim(CharT_(String)* self)
{
  CharT_(StringData)* d = self->_d;
  size_t length = d->length;

  if (length == 0)
    return ERR_OK;

  const CharT* sCur = d->data;
  const CharT* sEnd = sCur + length;

  while (sCur != sEnd   && CharT_Func::isSpace(*sCur)) sCur++;
  while (sCur != sEnd-- && CharT_Func::isSpace(*sEnd)) continue;

  if (sCur != d->data || ++sEnd != d->data + length)
  {
    length = (size_t)(sEnd - sCur);

    if (d->reference.get() > 1)
    {
      d = CharI_(String)::_dCreate(length, CharT_(Stub)(sCur, length));
      if (FOG_IS_NULL(d))
        return ERR_RT_OUT_OF_MEMORY;

      atomicPtrXchg(&self->_d, d)->release();
    }
    else
    {
      if (sCur != d->data)
        StringT_chcopy(d->data, sCur, length);

      d->hashCode = 0;
      d->length = length;
      d->data[length] = 0;
    }
  }

  return ERR_OK;
}

template<typename CharT>
static err_t FOG_CDECL StringT_simplify(CharT_(String)* self)
{
  CharT_(StringData)* d = self->_d;
  size_t length = d->length;

  if (length == 0)
    return ERR_OK;

  const CharT* sBegin;
  const CharT* sCur = d->data;
  const CharT* sEnd = sCur + length;

  CharT* dst;

  while (sCur != sEnd   && CharT_Func::isSpace(*sCur)) sCur++;
  while (sCur != sEnd-- && CharT_Func::isSpace(*sEnd)) continue;

  sBegin = sCur;

  // Left and Right trim is complete...

  if (sCur != d->data || sEnd + 1 != d->data + length)
    goto _Simplify;

  while (sCur < sEnd)
  {
    if (CharT_Func::isSpace(sCur[0]) && CharT_Func::isSpace(sCur[1]))
      goto _Simplify;
    sCur++;
  }
  return ERR_OK;

_Simplify:
  sCur = sBegin;
  sEnd++;

  if (d->reference.get() > 1)
  {
    d = CharI_(String)::_dCreate((size_t)(sEnd - sCur));
    if (FOG_IS_NULL(d))
      return ERR_RT_OUT_OF_MEMORY;
  }

  dst = d->data;

  do {
    if    (sCur != sEnd &&  CharT_Func::isSpace(*sCur)) *dst++ = ' ';
    while (sCur != sEnd &&  CharT_Func::isSpace(*sCur)) sCur++;
    while (sCur != sEnd && !CharT_Func::isSpace(*sCur)) *dst++ = *sCur++;
  } while (sCur != sEnd);

  length = (size_t)(dst - d->data);

  d->hashCode = 0;
  d->length = length;
  d->data[length] = 0;

  if (self->_d != d)
    atomicPtrXchg(&self->_d, d)->release();
  return ERR_OK;
}

// ============================================================================
// [Fog::String - Justify]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_justify(CharT_(String)* self, size_t n, CharT_Type ch, uint32_t flags)
{
  CharT_(StringData)* d = self->_d;
  size_t dLength = d->length;

  // If we are too large then there is nothing we can do.
  if (n <= dLength)
    return ERR_OK;

  size_t t = n - dLength;
  size_t left = 0;
  size_t right = 0;

  if ((flags & TEXT_JUSTIFY_CENTER) == TEXT_JUSTIFY_CENTER)
  {
    left = t >> 1;
    right = t - left;
  }
  else if ((flags & TEXT_JUSTIFY_LEFT) == TEXT_JUSTIFY_LEFT)
  {
    right = t;
  }
  else if ((flags & TEXT_JUSTIFY_RIGHT) == TEXT_JUSTIFY_RIGHT)
  {
    left = t;
  }

  if (d->reference.get() > 1 || d->capacity < n)
  {
    FOG_RETURN_ON_ERROR(self->reserve(n));
    d = self->_d;
  }

  CharT* p = d->data;

  if (left != 0)
  {
    StringT_chmove(p + left, p, dLength);
    StringT_chfill(p, ch, left);
  }
  if (right != 0)
  {
    StringT_chfill(p + n - right, ch, right);
  }

  d->hashCode = 0;
  d->length = n;
  d->data[n] = 0;
  return ERR_OK;
}

// ============================================================================
// [Fog::String - Split]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_splitChar(List<CharT_(String)>* dst, uint32_t cntOp, const CharT_(String)* src, const Range* range, CharT_Type ch, uint32_t splitBehavior, uint32_t cs)
{
  CharT_(StringData)* d = src->_d;

  size_t dLength = d->length;
  size_t rStart, rEnd;

  if (cntOp == CONTAINER_OP_REPLACE)
    dst->clear();

  if (!Range::fit(rStart, rEnd, dLength, range))
    return ERR_OK;

  const CharT* dData = d->data;
  const CharT* dCur = dData;
  const CharT* dEnd = dData + dLength;

  if (cs == CASE_SENSITIVE)
  {
_CaseSensitive:
    for (;;)
    {
      if (dCur == dEnd || *dCur == ch)
      {
        size_t splitLength = (size_t)(dCur - dData);

        if (splitLength != 0 || (splitLength == 0 && splitBehavior == SPLIT_KEEP_EMPTY))
          FOG_RETURN_ON_ERROR(dst->append(CharT_(Stub)(dData, splitLength)));

        if (dCur == dEnd) break;
        dData = ++dCur;
      }
      else
        dCur++;
    }
  }
  else
  {
    CharT_Value cArray[4];
    switch (StringT_getCaseInsensitiveChars<CharT_Value, CharT>(cArray, CharT(ch)))
    {
      case 1:
        goto _CaseSensitive;

      case 2:
        for (;;)
        {
          if (dCur == dEnd || *dCur == cArray[0] || *dCur == cArray[1])
          {
            size_t splitLength = (size_t)(dCur - dData);

            if (splitLength != 0 || (splitLength == 0 && splitBehavior == SPLIT_KEEP_EMPTY))
              FOG_RETURN_ON_ERROR(dst->append(CharT_(Stub)(dData, splitLength)));

            if (dCur == dEnd) break;
            dData = ++dCur;
          }
          else
            dCur++;
        }
        break;

      case 3:
        FOG_ASSERT(sizeof(CharT) != 1);

        for (;;)
        {
          if (dCur == dEnd || *dCur == cArray[0] || *dCur == cArray[1] || *dCur == cArray[2])
          {
            size_t splitLength = (size_t)(dCur - dData);

            if (splitLength != 0 || (splitLength == 0 && splitBehavior == SPLIT_KEEP_EMPTY))
              FOG_RETURN_ON_ERROR(dst->append(CharT_(Stub)(dData, splitLength)));

            if (dCur == dEnd) break;
            dData = ++dCur;
          }
          else
            dCur++;
        }
        break;

      default:
        FOG_ASSERT_NOT_REACHED();
    }
  }

  return ERR_OK;
}

template<typename CharT>
static err_t FOG_CDECL StringT_splitString(List<CharT_(String)>* dst, uint32_t cntOp, const CharT_(String)* src, const Range* range, const CharT_(String)* pattern, uint32_t splitBehavior, uint32_t cs)
{
  // Prevent memory corruption in case the the 'src' or 'pattern' was given
  // from the 'dst' list using dst->getAt() and passed directly to us.
  CharT_(String) srcCopy(*src);
  CharT_(String) patternCopy(*pattern);

  if (cntOp == CONTAINER_OP_REPLACE)
    dst->clear();
  size_t dstInitialLength = dst->getLength();

  size_t sLength = srcCopy.getLength();
  size_t rStart, rEnd;

  if (!Range::fit(rStart, rEnd, sLength, range))
    return ERR_OK;

  size_t pLength = patternCopy.getLength();

  const CharT* sData = srcCopy.getData() + rStart;
  const CharT* sEnd = srcCopy.getData() + rEnd;

  const CharT* pData = patternCopy.getData();

  for (;;)
  {
    size_t remain = (size_t)(sEnd - sData);
    size_t index = StringUtil::indexOf(sData, (size_t)(sEnd - sData), pData, pLength, cs);

    size_t splitLength = index != INVALID_INDEX ? index : remain;
    if ((splitLength == 0 && splitBehavior == SPLIT_KEEP_EMPTY) || splitLength != 0)
    {
      if (dst->append(CharT_(Stub)(sData, splitLength)) != ERR_OK)
        goto _OutOfMemory;
    }

    if (index == INVALID_INDEX)
      break;

    sData += index;
    sData += pLength;
  }
  return ERR_OK;

_OutOfMemory:
  dst->slice(Range(0, dstInitialLength));
  return ERR_RT_OUT_OF_MEMORY;
}

template<typename CharT>
static err_t FOG_CDECL StringT_splitRegExp(List<CharT_(String)>* dst, uint32_t cntOp, const CharT_(String)* src, const Range* range, const CharT_(RegExp)* re, uint32_t splitBehavior)
{
  CharT_(String) srcCopy(*src);

  if (cntOp == CONTAINER_OP_REPLACE)
    dst->clear();
  size_t dstInitialLength = dst->getLength();

  size_t sLength = srcCopy.getLength();
  size_t sStart, sEnd;

  if (!Range::fit(sStart, sEnd, sLength, range))
    return ERR_OK;

  const CharT* sData = srcCopy.getData() + sStart;
  sLength = sEnd - sStart;

  size_t rIndex = 0;
  size_t rEnd = sLength;

  for (;;)
  {
    Range m = re->indexIn(sData, sLength, Range(rIndex, rEnd));

    //size_t remain = (size_t)(strEnd - strCur);
    //Range m = filter.match(strCur, remain, cs, Range(0, remain));

    size_t splitLength = ((m.getStart() != INVALID_INDEX) ? m.getStart() : sEnd) - rIndex;
    if ((splitLength == 0 && splitBehavior == SPLIT_KEEP_EMPTY) || splitLength != 0)
    {
      if (dst->append(CharT_(Stub)(sData + rIndex, splitLength)) != ERR_OK)
        goto _OutOfMemory;
    }

    if (m.getStart() == INVALID_INDEX)
      break;

    rIndex = m.getEnd();
  }
  return ERR_OK;

_OutOfMemory:
  dst->slice(Range(0, dstInitialLength));
  return ERR_RT_OUT_OF_MEMORY;
}

// ============================================================================
// [Fog::String - Slice]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_slice(CharT_(String)* self, const Range* range)
{
  CharT_(StringData)* d = self->_d;

  size_t dLength = d->length;
  size_t rStart, rEnd;

  if (!Range::fit(rStart, rEnd, dLength, range))
  {
    self->clear();
    return ERR_OK;
  }

  size_t finalLength = rEnd - rStart;
  CharT* dData = d->data;

  if (d->reference.get() > 1)
  {
    d = CharI_(String)::_dCreate(finalLength, CharT_(Stub)(dData  + rStart, finalLength));
    if (FOG_IS_NULL(d))
      return ERR_RT_OUT_OF_MEMORY;

    atomicPtrXchg(&self->_d, d)->release();
    return ERR_OK;
  }
  else
  {
    if (rStart != 0)
      StringT_chcopy(dData, dData + rStart, finalLength);

    d->hashCode = 0;
    d->length = finalLength;
    dData[finalLength] = 0;

    return ERR_OK;
  }
}

// ============================================================================
// [Fog::String - Join]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_joinStub(CharT_(String)* self, const CharT_(String)* list, size_t listLength, const CharT_(Stub)* sep)
{
  size_t i;
  size_t finalLength = 0;

  const CharT* separatorData = sep->getData();
  size_t separatorLength = sep->getLength();

  // First collect how many characters will be in the result.
  for (i = 0; i < listLength; i++)
  {
    size_t itemLength = list[i].getLength();

    // Prevent for possible overflow (shouldn't normally happen).
    if (i > 0)
    {
      if (!Math::canSum(finalLength, separatorLength))
        return ERR_RT_OUT_OF_MEMORY;
      finalLength += separatorLength;
    }

    // Prevent for possible overflow (shouldn't normally happen).
    if (!Math::canSum(finalLength, itemLength))
      return ERR_RT_OUT_OF_MEMORY;
    finalLength += itemLength;
  }

  // Alloc memory and join.
  CharT* p = self->_prepare(CONTAINER_OP_APPEND, finalLength);
  if (FOG_IS_NULL(p))
    return ERR_RT_OUT_OF_MEMORY;

  for (i = 0; i < listLength; i++)
  {
    size_t itemLength = list[i].getLength();

    if (i > 0)
    {
      StringT_chcopy(p, separatorData, separatorLength);
      p += separatorLength;
    }

    StringT_chcopy(p, list[i].getData(), itemLength);
    p += itemLength;
  }

  FOG_ASSERT(p == self->_d->data + finalLength);
  return ERR_OK;
}

template<typename CharT>
static err_t FOG_CDECL StringT_joinString(CharT_(String)* self, const CharT_(String)* list, size_t listLength, const CharT_(String)* sep)
{
  CharT_(Stub) stub(sep->getData(), sep->getLength());
  return StringT_joinStub<CharT>(self, list, listLength, &stub);
}

template<typename CharT>
static err_t FOG_CDECL StringT_joinChar(CharT_(String)* self, const CharT_(String)* list, size_t listLength, CharT_Type sep)
{
  CharT_(Stub) stub(&sep, 1);
  return StringT_joinStub<CharT>(self, list, listLength, &stub);
}

// ============================================================================
// [Fog::String - ToBool]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_parseBool(const CharT_(String)* self, bool* dst, uint32_t base, size_t* end, uint32_t* pFlags)
{
  CharT_(StringData)* d = self->_d;
  return StringUtil::parseBool(dst, d->data, d->length, end, pFlags);
}

// ============================================================================
// [Fog::String - ParseInt]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_parseI8(const CharT_(String)* self, int8_t* dst, uint32_t base, size_t* pEnd, uint32_t* pFlags)
{
  CharT_(StringData)* d = self->_d;
  return StringUtil::parseI8(dst, d->data, d->length, base, pEnd, pFlags);
}

template<typename CharT>
static err_t FOG_CDECL StringT_parseU8(const CharT_(String)* self, uint8_t* dst, uint32_t base, size_t* pEnd, uint32_t* pFlags)
{
  CharT_(StringData)* d = self->_d;
  return StringUtil::parseU8(dst, d->data, d->length, base, pEnd, pFlags);
}

template<typename CharT>
static err_t FOG_CDECL StringT_parseI16(const CharT_(String)* self, int16_t* dst, uint32_t base, size_t* pEnd, uint32_t* pFlags)
{
  CharT_(StringData)* d = self->_d;
  return StringUtil::parseI16(dst, d->data, d->length, base, pEnd, pFlags);
}

template<typename CharT>
static err_t FOG_CDECL StringT_parseU16(const CharT_(String)* self, uint16_t* dst, uint32_t base, size_t* pEnd, uint32_t* pFlags)
{
  CharT_(StringData)* d = self->_d;
  return StringUtil::parseU16(dst, d->data, d->length, base, pEnd, pFlags);
}

template<typename CharT>
static err_t FOG_CDECL StringT_parseI32(const CharT_(String)* self, int32_t* dst, uint32_t base, size_t* pEnd, uint32_t* pFlags)
{
  CharT_(StringData)* d = self->_d;
  return StringUtil::parseI32(dst, d->data, d->length, base, pEnd, pFlags);
}

template<typename CharT>
static err_t FOG_CDECL StringT_parseU32(const CharT_(String)* self, uint32_t* dst, uint32_t base, size_t* pEnd, uint32_t* pFlags)
{
  CharT_(StringData)* d = self->_d;
  return StringUtil::parseU32(dst, d->data, d->length, base, pEnd, pFlags);
}

template<typename CharT>
static err_t FOG_CDECL StringT_parseI64(const CharT_(String)* self, int64_t* dst, uint32_t base, size_t* pEnd, uint32_t* pFlags)
{
  CharT_(StringData)* d = self->_d;
  return StringUtil::parseI64(dst, d->data, d->length, base, pEnd, pFlags);
}

template<typename CharT>
static err_t FOG_CDECL StringT_parseU64(const CharT_(String)* self, uint64_t* dst, uint32_t base, size_t* pEnd, uint32_t* pFlags)
{
  CharT_(StringData)* d = self->_d;
  return StringUtil::parseU64(dst, d->data, d->length, base, pEnd, pFlags);
}

// ============================================================================
// [Fog::String - ParseReal]
// ============================================================================

template<typename CharT>
static err_t FOG_CDECL StringT_parseFloat(const CharT_(String)* self, float* dst, CharT_Type decimalPoint, size_t* pEnd, uint32_t* pFlags)
{
  CharT_(StringData)* d = self->_d;
  return StringUtil::parseReal(dst, d->data, d->length, CharT(decimalPoint), pEnd, pFlags);
}

template<typename CharT>
static err_t FOG_CDECL StringT_parseDouble(const CharT_(String)* self, double* dst, CharT_Type decimalPoint, size_t* pEnd, uint32_t* pFlags)
{
  CharT_(StringData)* d = self->_d;
  return StringUtil::parseReal(dst, d->data, d->length, CharT(decimalPoint), pEnd, pFlags);
}

// ============================================================================
// [Fog::String - CountOf]
// ============================================================================

template<typename CharT>
static size_t FOG_CDECL StringT_countOfChar(const CharT_(String)* self, const Range* range, CharT_Type ch, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;

  if (cs >= CASE_SENSITIVITY_COUNT)
    cs = CASE_INSENSITIVE;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return 0;
  else
    return StringUtil::countOf(d->data + rStart, rEnd - rStart, CharT(ch), cs);
}

template<typename CharT, typename SrcT>
static size_t FOG_CDECL StringT_countOfStub(const CharT_(String)* self, const Range* range, const SrcT_(Stub)* pattern, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;

  if (cs >= CASE_SENSITIVITY_COUNT)
    cs = CASE_INSENSITIVE;

  const SrcT* pData = pattern->getData();
  size_t pLength = pattern->getComputedLength();

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return 0;

  if (pLength == 0)
    return 0;

  if (pLength == 1)
    return self->countOf(Range(rStart, rEnd), CharT(pData[0]), cs);

  size_t counter = 0;
  const CharT* sData = d->data;

  for (;;)
  {
    size_t i = StringUtil::indexOf(sData + rStart, rEnd - rStart, pData, pLength, cs);
    if (i == INVALID_INDEX)
      break;

    rStart += i + pLength;
    counter++;
  }

  return counter;
}

template<typename CharT, typename SrcT>
static size_t FOG_CDECL StringT_countOfString(const CharT_(String)* self, const Range* range, const SrcT_(String)* pattern, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;

  const SrcT* pData = pattern->getData();
  size_t pLength = pattern->getLength();

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return 0;

  if (pLength == 0)
    return 0;
  if (pLength == 1)
    return self->countOf(Range(rStart, rEnd), pData[0], cs);

  size_t counter = 0;
  const CharT* sData = d->data;

  for (;;)
  {
    size_t i = StringUtil::indexOf(sData + rStart, rEnd - rStart, pData, pLength);
    if (i == INVALID_INDEX)
      break;

    rStart += i + pLength;
    counter++;
  }

  return counter;
}

template<typename CharT>
static size_t FOG_CDECL StringT_countOfRegExp(const CharT_(String)* self, const Range* range, const CharT_(RegExp)* re)
{
  size_t rStart, rEnd;
  size_t counter = 0;

  if (re->getType() == REGEXP_TYPE_NONE)
    return counter;

  if (!Range::fit(rStart, rEnd, self->getLength(), range))
    return counter;

  for (;;)
  {
    Range r = re->indexIn(*self, Range(rStart, rEnd));
    if (!r.isValid())
      break;

    rStart = r.getEnd();
    counter++;
  }

  return counter;
}

// ============================================================================
// [Fog::String - IndexOf]
// ============================================================================

template<typename CharT>
static size_t FOG_CDECL StringT_indexOfChar(const CharT_(String)* self, const Range* range, CharT_Type ch, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return INVALID_INDEX;

  if (cs >= CASE_SENSITIVITY_COUNT)
    cs = CASE_INSENSITIVE;

  size_t i = StringUtil::indexOf(d->data + rStart, rEnd - rStart, CharT(ch), cs);
  return i != INVALID_INDEX ? i + rStart : i;
}

template<typename CharT, typename SrcT>
static size_t FOG_CDECL StringT_indexOfStub(const CharT_(String)* self, const Range* range, const SrcT_(Stub)* pattern, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;

  const SrcT* pData = pattern->getData();
  size_t pLength = pattern->getComputedLength();

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return INVALID_INDEX;

  if (pLength == 0)
    return INVALID_INDEX;

  if (cs >= CASE_SENSITIVITY_COUNT)
    cs = CASE_INSENSITIVE;

  const CharT* sData = d->data;

  size_t i = (pLength == 1)
    ? StringUtil::indexOf(sData + rStart, rEnd - rStart, CharT(pData[0]), cs)
    : StringUtil::indexOf(sData + rStart, rEnd - rStart, pData, pLength, cs);
  return i != INVALID_INDEX ? i + rStart : i;
}

template<typename CharT, typename SrcT>
static size_t FOG_CDECL StringT_indexOfString(const CharT_(String)* self, const Range* range, const SrcT_(String)* pattern, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;

  const SrcT* pData = pattern->getData();
  size_t pLength = pattern->getLength();

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return INVALID_INDEX;

  if (pLength == 0)
    return INVALID_INDEX;

  if (cs >= CASE_SENSITIVITY_COUNT)
    cs = CASE_INSENSITIVE;

  const CharT* sData = d->data;

  size_t i = (pLength == 1)
    ? StringUtil::indexOf(sData + rStart, rEnd - rStart, pData[0], cs)
    : StringUtil::indexOf(sData + rStart, rEnd - rStart, pData, pLength, cs);
  return i != INVALID_INDEX ? i + rStart : i;
}

template<typename CharT>
static size_t FOG_CDECL StringT_indexOfRegExp(const CharT_(String)* self, const Range* range, const CharT_(RegExp)* re)
{
  CharT_(StringData)* d = self->_d;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return INVALID_INDEX;

  return re->indexIn(*self, Range(rStart, rEnd)).getStart();
}

template<typename CharT>
static size_t FOG_CDECL StringT_indexOfAnyChar(const CharT_(String)* self, const Range* range, const CharT* charArray, size_t charLength, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return INVALID_INDEX;

  if (cs >= CASE_SENSITIVITY_COUNT)
    cs = CASE_INSENSITIVE;

  return StringUtil::indexOfAny(d->data + rStart, rEnd - rStart, charArray, charLength, cs);
}

// ============================================================================
// [Fog::String - LastIndexOf]
// ============================================================================

template<typename CharT>
static size_t FOG_CDECL StringT_lastIndexOfChar(const CharT_(String)* self, const Range* range, CharT_Type ch, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return INVALID_INDEX;

  if (cs >= CASE_SENSITIVITY_COUNT)
    cs = CASE_INSENSITIVE;

  size_t i = StringUtil::lastIndexOf(d->data + rStart, rEnd - rStart, CharT(ch), cs);
  return i != INVALID_INDEX ? i + rStart : i;
}

template<typename CharT, typename SrcT>
static size_t FOG_CDECL StringT_lastIndexOfStub(const CharT_(String)* self, const Range* range, const SrcT_(Stub)* pattern, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;

  const SrcT* pData = pattern->getData();
  size_t pLength = pattern->getComputedLength();

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return INVALID_INDEX;

  if (pLength == 0)
    return INVALID_INDEX;

  if (cs >= CASE_SENSITIVITY_COUNT)
    cs = CASE_INSENSITIVE;

  const CharT* sData = d->data;

  size_t i = (pLength == 1)
    ? StringUtil::lastIndexOf(sData + rStart, rEnd - rStart, CharT(pData[0]), cs)
    : StringUtil::lastIndexOf(sData + rStart, rEnd - rStart, pData, pLength, cs);
  return i != INVALID_INDEX ? i + rStart : i;
}

template<typename CharT, typename SrcT>
static size_t FOG_CDECL StringT_lastIndexOfString(const CharT_(String)* self, const Range* range, const SrcT_(String)* pattern, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;

  const SrcT* pData = pattern->getData();
  size_t pLength = pattern->getLength();

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return INVALID_INDEX;

  if (pLength == 0)
    return INVALID_INDEX;

  if (cs >= CASE_SENSITIVITY_COUNT)
    cs = CASE_INSENSITIVE;

  const CharT* sData = d->data;

  size_t i = (pLength == 1)
    ? StringUtil::lastIndexOf(sData + rStart, rEnd - rStart, pData[0], cs)
    : StringUtil::lastIndexOf(sData + rStart, rEnd - rStart, pData, pLength, cs);
  return i != INVALID_INDEX ? i + rStart : i;
}

template<typename CharT>
static size_t FOG_CDECL StringT_lastIndexOfRegExp(const CharT_(String)* self, const Range* range, const CharT_(RegExp)* re)
{
  CharT_(StringData)* d = self->_d;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return INVALID_INDEX;

  return re->lastIndexIn(*self, Range(rStart, rEnd)).getStart();
}

template<typename CharT>
static size_t FOG_CDECL StringT_lastIndexOfAnyChar(const CharT_(String)* self, const Range* range, const CharT* charArray, size_t charLength, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return INVALID_INDEX;

  if (cs >= CASE_SENSITIVITY_COUNT)
    cs = CASE_INSENSITIVE;

  return StringUtil::lastIndexOfAny(d->data + rStart, rEnd - rStart, charArray, charLength, cs);
}

// ============================================================================
// [Fog::String - StartsWith]
// ============================================================================

template<typename CharT>
static bool FOG_CDECL StringT_startsWithChar(const CharT_(String)* self, CharT_Type ch, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;
  size_t dLength = d->length;

  if (dLength == 0)
    return false;

  CharT_Value first = d->data[0];
  if (cs == CASE_SENSITIVE)
    return first == ch;
  else
    return CharT_Func::toLower(first) == CharT_Func::toLower(ch);
}

template<typename CharT, typename SrcT>
static bool FOG_CDECL StringT_startsWithStub(const CharT_(String)* self, const SrcT_(Stub)* str, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;
  size_t dLength = d->length;

  const SrcT* sData = str->getData();
  size_t sLength = str->getComputedLength();

  if (dLength < sLength)
    return false;

  if (cs == CASE_SENSITIVE)
    return StringT_cheq(d->data, sData, sLength);
  else
    return StringT_cheqi(d->data, sData, sLength);
}

template<typename CharT, typename SrcT>
static bool FOG_CDECL StringT_startsWithString(const CharT_(String)* self, const SrcT_(String)* str, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;
  size_t dLength = d->length;

  const SrcT* sData = str->getData();
  size_t sLength = str->getLength();

  if (dLength < sLength)
    return false;

  if (cs == CASE_SENSITIVE)
    return StringT_cheq(d->data, sData, sLength);
  else
    return StringT_cheqi(d->data, sData, sLength);
}

template<typename CharT>
static bool FOG_CDECL StringT_startsWithRegExp(const CharT_(String)* self, const CharT_(RegExp)* re)
{
  CharT_(StringData)* d = self->_d;

  size_t dLength = d->length;
  size_t rLength = re->getFixedLength();

  if (rLength == INVALID_INDEX)
    rLength = dLength;
  else if (rLength > dLength)
    return false;

  return re->indexIn(*self, Range(0, rLength)).getStart() == 0;
}

// ============================================================================
// [Fog::String - EndsWith]
// ============================================================================

template<typename CharT>
static bool FOG_CDECL StringT_endsWithChar(const CharT_(String)* self, CharT_Type ch, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;
  size_t dLength = d->length;

  if (dLength == 0)
    return false;

  CharT_Value last = d->data[dLength - 1];
  if (cs == CASE_SENSITIVE)
    return last == ch;
  else
    return CharT_Func::toLower(last) == CharT_Func::toLower(ch);
}

template<typename CharT, typename SrcT>
static bool FOG_CDECL StringT_endsWithStub(const CharT_(String)* self, const SrcT_(Stub)* str, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;
  size_t length = d->length;

  const SrcT* sData = str->getData();
  size_t sLength = str->getComputedLength();

  if (length < sLength)
    return false;

  if (cs == CASE_SENSITIVE)
    return StringT_cheq(d->data + length - sLength, sData, sLength);
  else
    return StringT_cheqi(d->data + length - sLength, sData, sLength);
}

template<typename CharT, typename SrcT>
static bool FOG_CDECL StringT_endsWithString(const CharT_(String)* self, const SrcT_(String)* str, uint32_t cs)
{
  CharT_(StringData)* d = self->_d;
  size_t length = d->length;

  const SrcT* sData = str->getData();
  size_t sLength = str->getLength();

  if (length < sLength)
    return false;

  if (cs == CASE_SENSITIVE)
    return StringT_cheq(d->data + length - sLength, sData, sLength);
  else
    return StringT_cheqi(d->data + length - sLength, sData, sLength);
}

template<typename CharT>
static bool FOG_CDECL StringT_endsWithRegExp(const CharT_(String)* self, const CharT_(RegExp)* re)
{
  CharT_(StringData)* d = self->_d;
  return re->lastIndexIn(*self).getEnd() == d->length;
}

// ============================================================================
// [Fog::String - Equality]
// ============================================================================

template<typename CharT, typename SrcT>
static bool FOG_CDECL StringT_eqStub(const CharT_(String)* a, const SrcT_(Stub)* b)
{
  const CharT* aData = a->getData();
  const SrcT* bData = b->getData();

  size_t aLength = a->getLength();
  size_t bLength = b->getLength();

  if (bLength == DETECT_LENGTH)
  {
    for (size_t i = 0; i < aLength; i++)
    {
      if (bData[i] == 0 || aData[i] != (SrcT_Char::Value)bData[i])
        return false;
    }

    return bData[aLength] == 0;
  }
  else
  {
    return aLength == bLength && StringUtil::eq(aData, bData, aLength, CASE_SENSITIVE);
  }
}

template<typename CharT, typename SrcT>
static bool FOG_CDECL StringT_eqString(const CharT_(String)* a, const SrcT_(String)* b)
{
  const CharT_(StringData)* a_d = a->_d;
  const SrcT_(StringData)* b_d = b->_d;

  size_t aLength = a_d->length;
  size_t bLength = b_d->length;

  if (sizeof(CharT) == sizeof(SrcT) && (const void*)a_d == (const void*)b_d)
    return true;

  if (aLength != bLength)
    return false;

  if (sizeof(CharT) == sizeof(SrcT) && ((a_d->vType & b_d->vType) & VAR_FLAG_STRING_INTERNED) == VAR_FLAG_STRING_INTERNED)
    return false;

  return StringUtil::eq(a_d->data, b_d->data, aLength, CASE_SENSITIVE);
}

template<typename CharT, typename SrcT>
static bool FOG_CDECL StringT_eqStubEx(const CharT_(String)* a, const SrcT_(Stub)* b, uint32_t cs)
{
  const CharT* aData = a->getData();
  const SrcT* bData = b->getData();

  size_t aLength = a->getLength();
  size_t bLength = b->getLength();

  if (bLength == DETECT_LENGTH)
  {
    if (cs == CASE_SENSITIVE)
    {
      for (size_t i = 0; i < aLength; i++)
      {
        if (bData[i] == 0 || aData[i] != (SrcT_Char::Value)bData[i])
          return false;
      }
    }
    else
    {
      for (size_t i = 0; i < aLength; i++)
      {
        if (bData[i] == 0 || CharT_Func::toLower(aData[i]) != (SrcT_Char::Value)SrcI_(_Char)::toLower(bData[i]))
          return false;
      }
    }

    return bData[aLength] == 0;
  }
  else
  {
    return aLength == bLength && StringUtil::eq(aData, bData, aLength, CASE_SENSITIVE);
  }
}

template<typename CharT, typename SrcT>
static bool FOG_CDECL StringT_eqStringEx(const CharT_(String)* a, const SrcT_(String)* b, uint32_t cs)
{
  const CharT_(StringData)* a_d = a->_d;
  const SrcT_(StringData)* b_d = b->_d;

  size_t aLength = a_d->length;
  size_t bLength = b_d->length;

  if (a_d == b_d)
    return true;

  if (aLength != bLength)
    return false;

  if (cs >= CASE_SENSITIVITY_COUNT)
    cs = CASE_INSENSITIVE;

  return StringUtil::eq(a_d->data, b_d->data, aLength, cs);
}

// ============================================================================
// [Fog::String - Compare]
// ============================================================================

static FOG_INLINE int StringT_compareLength(size_t aLength, size_t bLength)
{
  if (aLength > bLength) return 1;
  if (aLength < bLength) return -1;

  return 0;
}

template<typename CharT, typename SrcT>
static FOG_INLINE int StringT_compareAgainstNullTerminated_cs(const CharT* aData, size_t aLength, const SrcT* bData)
{
  FOG_ASSERT(aLength > 0);
  const CharT* aEnd = aData + aLength;

  do {
    int c = static_cast<int>((CharT_Value)*aData) -
            static_cast<int>((SrcT_Char::Value)*bData);
    if (c != 0)
      return c;

    // The NULL terminator can be detected using the 'c' value. If 'b' is
    // zero and there is something in 'a' then the 'c' will be positive,
    // which matches the compare result. However, if there is a NULL char
    // inside 'a' then there is security issue of bypassed NULL terminator
    // in 'b'. We are not sure about NULL existence in 'a' so we need to
    // check 'b' again.
    if (*bData == 0)
      return 1;

    aData++;
    bData++;
  } while (aData != aEnd);
  return *bData ? -1 : 0;
}

template<typename CharT, typename SrcT>
static FOG_INLINE int StringT_compareAgainstNullTerminated_ci(const CharT* aData, size_t aLength, const SrcT* bData)
{
  FOG_ASSERT(aLength > 0);
  const CharT* aEnd = aData + aLength;

  do {
    int c = static_cast<int>((CharT_Value)CharT_Func::toLower(*aData)) -
            static_cast<int>((SrcT_Char::Value)SrcI_(_Char)::toLower(*bData));
    if (c != 0)
      return c;

    // The NULL terminator can be detected using the 'c' value. If 'b' is
    // zero and there is something in 'a' then the 'c' will be positive,
    // which matches the compare result. However, if there is a NULL char
    // inside 'a' then there is security issue of bypassed NULL terminator
    // in 'b'. We are not sure about NULL existence in 'a' so we need to
    // check 'b' again.
    if (*bData == 0)
      return 1;

    aData++;
    bData++;
  } while (aData != aEnd);
  return *bData ? -1 : 0;
}

template<typename CharT, typename SrcT>
static FOG_INLINE int StringT_compareRaw_cs(const CharT* aData, size_t aLength, const SrcT* bData, size_t bLength)
{
  FOG_ASSERT(aLength > 0);

  size_t sharedLength = Math::min(aLength, bLength);

  for (size_t i = 0; i < sharedLength; i++)
  {
    int c = static_cast<int>((CharT_Value)aData[i]) -
            static_cast<int>((SrcT_Char::Value)bData[i]);
    if (c != 0)
      return c;
  }

  return StringT_compareLength(aLength, bLength);
}

template<typename CharT, typename SrcT>
static FOG_INLINE int StringT_compareRaw_ci(const CharT* aData, size_t aLength, const SrcT* bData, size_t bLength)
{
  FOG_ASSERT(aLength > 0);

  size_t sharedLength = Math::min(aLength, bLength);

  for (size_t i = 0; i < sharedLength; i++)
  {
    int c = static_cast<int>((CharT_Value)CharT_Func::toLower(aData[i])) -
            static_cast<int>((SrcT_Char::Value)SrcI_(_Char)::toLower(bData[i]));
    if (c != 0)
      return c;
  }

  return StringT_compareLength(aLength, bLength);
}

template<typename CharT, typename SrcT>
static int FOG_CDECL StringT_compareStub(const CharT_(String)* a, const SrcT_(Stub)* b)
{
  size_t aLength = a->getLength();
  size_t bLength = b->getLength();

  const CharT* aData = a->getData();
  const SrcT* bData = b->getData();

  if (aLength == 0)
    return *bData ? -1 : 0;

  if (bLength == DETECT_LENGTH)
    return StringT_compareAgainstNullTerminated_cs<CharT, SrcT>(aData, aLength, bData);
  else
    return StringT_compareRaw_cs<CharT, SrcT>(aData, aLength, bData, bLength);
}

template<typename CharT, typename SrcT>
static int FOG_CDECL StringT_compareString(const CharT_(String)* a, const SrcT_(String)* b)
{
  size_t aLength = a->getLength();
  size_t bLength = b->getLength();

  const CharT* aData = a->getData();
  const SrcT* bData = b->getData();

  return StringT_compareRaw_cs<CharT, SrcT>(aData, aLength, bData, bLength);
}

template<typename CharT, typename SrcT>
static int FOG_CDECL StringT_compareStubEx(const CharT_(String)* a, const SrcT_(Stub)* b, uint32_t cs)
{
  size_t aLength = a->getLength();
  size_t bLength = b->getLength();

  const CharT* aData = a->getData();
  const CharT* aEnd  = aData + aLength;
  const SrcT* bData = b->getData();

  if (FOG_UNLIKELY(aData == aEnd))
    return *bData ? -1 : 0;

  if (bLength == DETECT_LENGTH)
  {
    if (cs == CASE_SENSITIVE)
      return StringT_compareAgainstNullTerminated_cs<CharT, SrcT>(aData, aLength, bData);
    else
      return StringT_compareAgainstNullTerminated_ci<CharT, SrcT>(aData, aLength, bData);
  }
  else
  {
    if (cs == CASE_SENSITIVE)
      return StringT_compareRaw_cs<CharT, SrcT>(aData, aLength, bData, bLength);
    else
      return StringT_compareRaw_ci<CharT, SrcT>(aData, aLength, bData, bLength);
  }
}

template<typename CharT, typename SrcT>
static int FOG_CDECL StringT_compareStringEx(const CharT_(String)* a, const SrcT_(String)* b, uint32_t cs)
{
  size_t aLength = a->getLength();
  size_t bLength = b->getLength();

  const CharT* aData = a->getData();
  const SrcT* bData = b->getData();

  if (cs == CASE_SENSITIVE)
    return StringT_compareRaw_cs<CharT, SrcT>(aData, aLength, bData, bLength);
  else
    return StringT_compareRaw_ci<CharT, SrcT>(aData, aLength, bData, bLength);
}

// ============================================================================
// [Fog::String - Validate]
// ============================================================================

static err_t FOG_CDECL StringA_validateUtf8(const StringA* self, size_t* invalid)
{
  StringDataA* d = self->_d;
  return StringUtil::validateUtf8(d->data, d->length, invalid);
}

static err_t FOG_CDECL StringW_validateUtf16(const StringW* self, size_t* invalid)
{
  StringDataW* d = self->_d;
  return StringUtil::validateUtf16(d->data, d->length, invalid);
}

// ============================================================================
// [Fog::String - Canonication]
// ============================================================================

static bool FOG_CDECL StringW_isCanonical(const StringW* self, const Range* range, uint32_t version, size_t* where)
{
  StringDataW* d = self->_d;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return true;

  size_t whereDummy;
  if (where == NULL)
    where = &whereDummy;

  if (!StringUtil::isCanonical(d->data + rStart, rEnd - rStart, version, where))
  {
    *where += rStart;
    return false;
  }
  else
  {
    return true;
  }
}

static err_t FOG_CDECL StringW_makeCanonical(StringW* self, const Range* range, uint32_t version)
{
  StringDataW* d = self->_d;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return ERR_OK;

  size_t from;
  if (StringUtil::isCanonical(d->data + rStart, rEnd - rStart, version, &from))
    return ERR_OK;

  FOG_RETURN_ON_ERROR(self->detach());
  d = self->_d;

  StringUtil::makeCanonical(d->data + rStart, rEnd - rStart, version, from);
  
  d->hashCode = 0;
  return ERR_OK;
}

// ============================================================================
// [Fog::String - Normalization]
// ============================================================================

static bool FOG_CDECL StringW_isNormalized(const StringW* self, const Range* range, uint32_t normForm, uint32_t version, size_t* where)
{
  if (normForm >= CHAR_NORMALIZATION_FORM_COUNT)
    return false;

  StringDataW* d = self->_d;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return ERR_OK;

  size_t localWhere;
  bool result = StringUtil::isNormalized(d->data + rStart, rEnd - rStart, normForm, version, &localWhere);

  if (where)
    *where = localWhere + rStart;

  return result;
}

static uint32_t FOG_CDECL StringW_quickCheck(const StringW* self, const Range* range, uint32_t normForm, uint32_t version, size_t* where)
{
  if (normForm >= CHAR_NORMALIZATION_FORM_COUNT)
    return CHAR_QUICK_CHECK_NO;

  StringDataW* d = self->_d;

  size_t rStart, rEnd;
  if (!Range::fit(rStart, rEnd, d->length, range))
    return ERR_OK;

  size_t localWhere;
  uint32_t result = StringUtil::quickCheck(d->data + rStart, rEnd - rStart, normForm, version, &localWhere);

  if (where)
    *where = localWhere + rStart;

  return result;
}

struct CharDecomposeBuffer
{
  // Maximum count of characters after decomposition is 18.
  uint32_t utf32[18];
  uint32_t length;
  uint32_t pos;
};

// We always add this free space to the buffer so we can decompose the whole 
// character recursively without performing end of buffer checks.
enum { CHAR_MAX_DECOMPOSITION_LENGTH = 18 };

static err_t FOG_CDECL StringW_decomposePrivate(StringW* dst, const uint16_t* sData, size_t sLength, bool canonical, uint32_t version)
{
  size_t dstGuess = sLength;
  if (dstGuess < SIZE_MAX / 2)
    dstGuess *= 2;
  if (dstGuess < SIZE_MAX - CHAR_MAX_DECOMPOSITION_LENGTH)
    dstGuess += 18;

  uint16_t* dData;
  uint16_t* dEnd;

  dData = reinterpret_cast<uint16_t*>(dst->_prepare(CONTAINER_OP_REPLACE, dstGuess));
  if (FOG_IS_NULL(dData))
    return ERR_RT_OUT_OF_MEMORY;
  dEnd = dData + dst->getCapacity() - CHAR_MAX_DECOMPOSITION_LENGTH;

  const uint16_t* sEnd = sData + sLength;
  CharDecomposeBuffer bufStorage[8];

  while (sData != sEnd)
  {
    if (FOG_UNLIKELY(dData >= dEnd))
    {
      dst->_modified(dData);

      dstGuess = (size_t)(sEnd - sData);
      if (dstGuess < SIZE_MAX / 2)
        dstGuess *= 2;
      if (dstGuess < SIZE_MAX - CHAR_MAX_DECOMPOSITION_LENGTH)
        dstGuess += 18;

      dData = reinterpret_cast<uint16_t*>(dst->_add(dstGuess));
      if (FOG_IS_NULL(dData))
      {
        dst->clear();
        return ERR_RT_OUT_OF_MEMORY;
      }

      dEnd = reinterpret_cast<uint16_t*>(dst->getDataX() + dst->getCapacity() - CHAR_MAX_DECOMPOSITION_LENGTH);
    }

    uint32_t c0 = *sData++;
    uint16_t c1;

    dData[0] = static_cast<uint16_t>(c0);

    if (CharW::isHiSurrogate(c0) && sData != sEnd && CharW::isLoSurrogate((c1 = sData[0])))
    {
      c0 = CharW::ucs4FromSurrogate(c0, static_cast<uint32_t>(c1));

      dData[1] = c1;
      sData++;
    }

    uint32_t decompositionType;
    uint32_t decompositionLength = CharW::decompose(c0, bufStorage[0].utf32, &decompositionType);

    if (decompositionLength == 0 || (canonical && decompositionType != CHAR_DECOMPOSITION_CANONICAL))
    {
      // If no decomposition was done then just adjust dData pointer. We
      // already stored the incoming character in the buffer, so adjustion
      // is the only needed operation to complete this character.
      dData += 1 + CharW::ucs4IsSMP(c0);
    }
    else
    {
      // Okay, character was decomposed, so decompose it recursively until
      // there is no decomposition. We use UTF-32 buffer from now, because
      // it saves us from working with surrogates.
      CharDecomposeBuffer* p = bufStorage;

      p->length = decompositionLength;
      p->pos = 0;

      for (;;)
      {
        decompositionLength = CharW::decompose(p->utf32[p->pos], p[1].utf32, &decompositionType);
        if (decompositionLength == 0 || (canonical && decompositionType != CHAR_DECOMPOSITION_CANONICAL))
        {
          c0 = p->utf32[p->pos++];
          if (CharW::ucs4IsBMP(c0))
          {
            dData[0] = static_cast<uint16_t>(c0);
            dData += 1;
          }
          else
          {
            CharW::ucs4ToSurrogate(&dData[0], &dData[1], c0);
            dData += 2;
          }

          bool complete = false;
          while (p->pos >= p->length)
          {
            complete = (p == bufStorage);
            if (complete)
              break;
            p--;
          }

          if (complete)
            break;
        }
        else
        {
          p->pos++;
          p++;
          FOG_ASSERT(p != bufStorage + FOG_ARRAY_SIZE(bufStorage) - 1);

          p->length = decompositionLength;
          p->pos = 0;
        }
      }
    }
  }

  dst->_modified(dData);
  return ERR_OK;
}

static size_t FOG_CDECL StringW_composePrivate(uint16_t* dData, const uint16_t* sData, size_t sLength)
{
  if (sLength == 0)
    return 0;

  uint16_t* dBegin = dData;
  const uint16_t* sEnd = sData + sLength;

  for (;;)
  {
    uint32_t cA = sData[0];
    uint32_t cB;

_Next:
    if (++sData == sEnd)
    {
      *dData++ = static_cast<uint16_t>(cA);
      break;
    }

    // Handle composition of Hangul L+V or L+V+T syllables. All Hangul 
    // characters are in BMP, so we don't need to check for surrogates.
    // We also know that the hangul jamos should be in L+V+T format,
    // because of canonical decomposition, thus, we expect exactly this
    // order: [L+V] or [L+V+T].
    if (Math::isBounded<uint32_t>(cA, CHAR_HANGUL_BASE_L, CHAR_HANGUL_BASE_L + CHAR_HANGUL_COUNT_L - 1))
    {
      cB = sData[0];
      if (Math::isBounded<uint32_t>(cB, CHAR_HANGUL_BASE_V, CHAR_HANGUL_BASE_V + CHAR_HANGUL_COUNT_V - 1))
      {
        // Compose L+V jamos to LV syllable.
        cA = CHAR_HANGUL_BASE_S + (cA - CHAR_HANGUL_BASE_L) * (CHAR_HANGUL_COUNT_N    ) + 
                                  (cB - CHAR_HANGUL_BASE_V) * (CHAR_HANGUL_COUNT_T + 1) ;

        if (++sData != sEnd)
        {
          cB = sData[0];

          // Compose LV syllable and T jamo to LVT syllable.
          if (Math::isBounded<uint32_t>(cB, CHAR_HANGUL_BASE_T, CHAR_HANGUL_BASE_T + CHAR_HANGUL_COUNT_T - 1))
          {
            cA += cB - (CHAR_HANGUL_BASE_T - 1);
            sData++;
          }
        }

        // Write LV or LVT syllable to the destination. The syllable is final,
        // there is nothing which can change it.
        *dData++ = static_cast<uint16_t>(cA);
      }
    }
    else
    {
      cB = sData[0];

      if (CharW::isHiSurrogate(cA) && CharW::isLoSurrogate(cB))
      {
        if (++sData == sEnd)
        {
          dData[0] = static_cast<uint16_t>(cA);
          dData[1] = static_cast<uint16_t>(cB);
          dData += 2;
          break;
        }

        cA = CharW::ucs4FromSurrogate(cA, cB);
        cB = sData[0];
      }

      const CharProperty& pA = CharW::getPropertyUnsafe(cA);
      if (!pA.hasComposition())
      {
_EmitAndNext:
        if (CharW::ucs4IsBMP(cA))
        {
          *dData++ = static_cast<uint16_t>(cA);
        }
        else
        {
          CharW::ucs4ToSurrogate(&dData[0], &dData[1], cA);
          dData += 2;
        }

        cA = cB;
        goto _Next;
      }

      sData++;

      if (CharW::isHiSurrogate(cB))
      {
        if (sData == sEnd)
        {
          sData--;
          goto _EmitAndNext;
        }

        if (CharW::isLoSurrogate(sData[0]))
          cB = CharW::ucs4FromSurrogate(cB, static_cast<uint32_t>(*sData++));
      }

      // Non-composited characters (combiners) array.
      uint16_t* nBegin = dData + 1 + CharW::ucs4IsSMP(cA);
      uint16_t* nPtr = nBegin;

      for (;;)
      {
        const CharProperty& pB = CharW::getPropertyUnsafe(cB);
        uint32_t cAB = pB.getCompositionId();

        if (cAB == 0)
        {
          break;
        }
        else
        {
          uint32_t cStart = _charData.compositionIdToIndex[cAB];
          uint32_t cEnd = _charData.compositionIdToIndex[cAB + 1];

          cAB = CharUtilPrivate::composeBSearch(_charData.compositionData + cStart, cEnd - cStart, cA);
          if (cAB != 0)
          {
            cA = cAB;
          }
          else if (CharW::ucs4IsBMP(cB))
          {
            *nPtr++ = static_cast<uint16_t>(cB);
          }
          else
          {
            CharW::ucs4ToSurrogate(&nPtr[0], &nPtr[1], cB);
            nPtr += 2;
          }
        }

        if (sData == sEnd)
          break;

        cB = *sData++;
        if (CharW::isHiSurrogate(cB))
        {
          if (sData == sEnd || !CharW::isLoSurrogate(sData[0]))
          {
            *nPtr++ = static_cast<uint16_t>(cB);
            break;
          }

          cB = CharW::ucs4FromSurrogate(cB, static_cast<uint32_t>(*sData++));
        }
      }

      if (CharW::ucs4IsBMP(cA))
      {
        *dData++ = static_cast<uint16_t>(cA);

        // I don't know whether it's possible that the composition of characters
        // in BMP can result in composition of characters in SMP. However, to be
        // safe and to support possible future extensions this code can handle it.
        if (dData != nBegin)
        {
          while (nBegin != nPtr)
            *dData++ = *nBegin++;
        }
        else
        {
          dData = nPtr;
        }
      }
      else
      {
        size_t nLength = (size_t)(nPtr - nBegin);
        // The same as the previous comment.
        if (dData + 2 != nBegin)
        {
          for (size_t i = nLength; i; i--)
            dData[i+1] = dData[i];
        }

        CharW::ucs4ToSurrogate(&dData[0], &dData[1], cA);
        dData += 2 + nLength;
      }

      if (sData == sEnd)
        break;
    }
  }

  return (size_t)(dData - dBegin);
}

static err_t FOG_CDECL StringW_normalize(StringW* self, const Range* range, uint32_t normForm, uint32_t version)
{
  StringTmpW<TEMPORARY_LENGTH> sTmp;

  if (normForm >= CHAR_NORMALIZATION_FORM_COUNT)
    return ERR_RT_INVALID_ARGUMENT;

  StringDataW* d = self->_d;
  size_t rStart, rEnd;
  
  if (!Range::fit(rStart, rEnd, d->length, range))
    return ERR_OK;

  // Decompose string into NFD or NFKD.
  size_t rLength = rEnd - rStart;
  err_t err = StringW_decomposePrivate(&sTmp, reinterpret_cast<const uint16_t*>(d->data) + rStart, rLength, normForm < CHAR_NFKD, version);

  if (FOG_IS_ERROR(err))
    return err;

  // Make canonical ordering of combining characters.
  StringUtil::makeCanonical(sTmp._d->data, sTmp._d->length, version);

  // If the requested normalization form was NFD/NFKD then we are done.
  if (normForm == CHAR_NFD || normForm == CHAR_NFKD)
    return self->replace(Range(rStart, rEnd), sTmp);

  // Compose to NFC/NFKC form.
  uint16_t* p = reinterpret_cast<uint16_t*>(sTmp.getDataX());
  sTmp._modified(p + StringW_composePrivate(p, p, sTmp.getLength()));

  return self->replace(Range(rStart, rEnd), sTmp);
}

// ============================================================================
// [Fog::String - GetUcsLength]
// ============================================================================

static err_t FOG_CDECL StringA_getUcsLength(const StringA* self, size_t* ucsLength)
{
  StringDataA* d = self->_d;
  return StringUtil::ucsFromUtf8Length(d->data, d->length, ucsLength);
}

static err_t FOG_CDECL StringW_getUcsLength(const StringW* self, size_t* ucsLength)
{
  StringDataW* d = self->_d;
  return StringUtil::ucsFromUtf16Length(d->data, d->length, ucsLength);
}

// ============================================================================
// [Fog::String - Hex]
// ============================================================================

static err_t FOG_CDECL StringA_hexDecode(StringA* dst, uint32_t cntOp, const StringA* src)
{
  StringDataA* sd = src->_d;
  size_t sLength = sd->length;
  size_t dLength = (sLength >> 1) + (sLength & 1);

  // Prevent data corruption in case that dst == src.
  sd->addRef();

  uint8_t* dData = reinterpret_cast<uint8_t*>(dst->_prepare(cntOp, dLength));
  const uint8_t* sData = reinterpret_cast<const uint8_t*>(sd->data);

  if (FOG_IS_NULL(dData))
    return ERR_RT_OUT_OF_MEMORY;

  uint8_t c0 = 0xFF;
  uint8_t c1;

  for (size_t i = 0; i < sLength; i++)
  {
    c1 = sData[i];

    if (c1 >= '0' && c1 <= '9')
      c1 -= '0';
    else if (c1 >= 'a' && c1 <= 'f')
      c1 -= ('a' - 10);
    else if (c1 >= 'A' && c1 <= 'F')
      c1 -= ('A' + 10);
    else
      continue;

    if (c0 == 0xFF)
    {
      c0 = c1;
    }
    else
    {
      *dData++ = (c0 << 4) | c1;
      c0 = 0xFF;
    }
  }

  sd->release();

  dst->_modified(reinterpret_cast<char*>(dData));
  return ERR_OK;
}

static err_t FOG_CDECL StringA_hexEncode(StringA* dst, uint32_t cntOp, const StringA* src, uint32_t textCase)
{
  StringDataA* sd = src->_d;
  size_t sLength = sd->length;
  size_t dLength = sLength << 1;

  if (dLength < sLength)
    return ERR_RT_OUT_OF_MEMORY;

  // Prevent data corruption in case that dst == src.
  sd->addRef();

  uint8_t* dData = reinterpret_cast<uint8_t*>(dst->_prepare(cntOp, dLength));
  if (FOG_IS_NULL(dData)) return ERR_RT_OUT_OF_MEMORY;
  const uint8_t* sData = reinterpret_cast<const uint8_t*>(sd->data);

  uint8_t c0;
  uint8_t c1;

  uint8_t hx = (textCase == TEXT_CASE_LOWER)
    ? (uint8_t)'a' - ((uint8_t)'9' + 1U)
    : (uint8_t)'A' - ((uint8_t)'9' + 1U);

  for (size_t i = sLength; i; i--)
  {
    c0 = *sData++;
    c1 = c0;

    c0 >>= 4;
    c1 &= 0x0F;

    c0 += '0';
    c1 += '0';

    if (c0 > (uint8_t)'9') c0 += hx;
    if (c1 > (uint8_t)'9') c1 += hx;

    dData[0] = c0;
    dData[1] = c1;
    dData += 2;
  }

  sd->release();

  dst->_modified(reinterpret_cast<char*>(dData));
  return ERR_OK;
}

// ============================================================================
// [Fog::String - Base64]
// ============================================================================

static const char StringT_base64Tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char StringT_base64Pad = '=';

static err_t FOG_CDECL StringA_base64DecodeStringA(StringA* dst, uint32_t cntOp, const StringA* src)
{
  if (dst == src)
  {
    StringA copy(*src);
    return fog_api.stringa_base64DecodeDataA(dst, cntOp, copy.getData(), copy.getLength());
  }
  else
  {
    return fog_api.stringa_base64DecodeDataA(dst, cntOp, src->getData(), src->getLength());
  }
}

static err_t FOG_CDECL StringA_base64DecodeStringW(StringA* dst, uint32_t cntOp, const StringW* src)
{
  return fog_api.stringa_base64DecodeDataW(dst, cntOp, src->getData(), src->getLength());
}

template<typename SrcT>
static err_t FOG_CDECL StringA_base64DecodeData(StringA* dst, uint32_t cntOp, const SrcT* src, size_t sLength)
{
  if (sLength == DETECT_LENGTH)
    sLength = StringUtil::len(src);
  size_t dLength = (sLength / 4) * 3 + 3;

  uint8_t* dData = reinterpret_cast<uint8_t*>(dst->_prepare(cntOp, dLength));
  const SrcT_Char::Value* sData = reinterpret_cast<const SrcT_Char::Value*>(src);

  if (FOG_IS_NULL(dData))
    return ERR_RT_OUT_OF_MEMORY;

  uint32_t accum = 0;
  uint32_t bits = 0;
  uint32_t c0;

  for (size_t i = 0; i < sLength; i++)
  {
    c0 = sData[i];

    if (c0 >= '0' && c0 <= '9')
      c0 -= ('0' - 52);
    else if (c0 >= 'a' && c0 <= 'z')
      c0 -= ('a' - 26);
    else if (c0 >= 'A' && c0 <= 'Z')
      c0 -= 'A';
    else if (c0 == '+')
      c0 = 62;
    else if (c0 == '/')
      c0 = 63;
    else
      continue;

    accum = (accum << 6) | c0;
    if (bits >= 2)
    {
      bits -= 2;
      *dData++ = (uint8_t)(accum >> bits);
    }
    else
    {
      bits += 6;
    }
  }

  dst->_modified(reinterpret_cast<char*>(dData));
  return ERR_OK;
}

static err_t FOG_CDECL StringA_base64EncodeStringA(StringA* dst, uint32_t cntOp, const StringA* src)
{
  if (dst == src)
  {
    StringA copy(*src);
    return fog_api.stringa_base64EncodeDataA(dst, cntOp, copy.getData(), copy.getLength());
  }
  else
  {
    return fog_api.stringa_base64EncodeDataA(dst, cntOp, src->getData(), src->getLength());
  }
}

static err_t FOG_CDECL StringW_base64EncodeStringA(StringW* dst, uint32_t cntOp, const StringA* src)
{
  return fog_api.stringw_base64EncodeDataA(dst, cntOp, src->getData(), src->getLength());
}

template<typename CharT>
static err_t FOG_CDECL StringT_base64EncodeDataA(CharT_(String)* dst, uint32_t cntOp, const char* src, size_t sLength)
{
  if (sLength == DETECT_LENGTH)
    sLength = StringUtil::len(src);

  if (sLength > SIZE_MAX / 4)
    return ERR_RT_OUT_OF_MEMORY;

  size_t dLength = (size_t)( ((uint64_t)sLength * 4) / 3 + 3 );
  if (dLength < sLength)
    return ERR_RT_OUT_OF_MEMORY;

  CharT* dData = dst->_prepare(cntOp, dLength);
  const uint8_t* sData = reinterpret_cast<const uint8_t*>(src);

  if (FOG_IS_NULL(dData))
    return ERR_RT_OUT_OF_MEMORY;

  size_t i = sLength;
  while (i >= 3)
  {
    uint8_t c0 = sData[0];
    uint8_t c1 = sData[1];
    uint8_t c2 = sData[2];

    dData[0] = StringT_base64Tab[((c0 & 0xFC) >> 2)];
    dData[1] = StringT_base64Tab[((c0 & 0x03) << 4) + ((c1 & 0xF0) >> 4)];
    dData[2] = StringT_base64Tab[((c1 & 0x0F) << 2) + ((c2 & 0xC0) >> 6)];
    dData[3] = StringT_base64Tab[((c2 & 0x3f))];

    sData += 3;
    dData += 4;

    i -= 3;
  }

  if (i)
  {
    uint8_t c0 = sData[0];
    uint8_t c1 = (i > 1) ? sData[1] : 0;
    uint8_t c2 = (i > 2) ? sData[2] : 0;

    dData[0] = StringT_base64Tab[((c0 & 0xFC) >> 2)];
    dData[1] = StringT_base64Tab[((c0 & 0x03) << 4) + ((c1 & 0xF0) >> 4)];
    dData[2] = (i > 1) ? StringT_base64Tab[((c1 & 0x0F) << 2) + ((c2 & 0xC0) >> 6)]
                       : StringT_base64Pad;
    // 'i' shouldn't be larger than 2, but...
    dData[3] = (i > 2) ? StringT_base64Tab[((c2 & 0x3f))]
                       : StringT_base64Pad;

    dData += 4;
    i -= 3;
  }

  dst->_modified(dData);
  return ERR_OK;
}

// ============================================================================
// [Fog::String - BSwap]
// ============================================================================

static err_t FOG_CDECL StringW_bswap(StringW* self)
{
  StringDataW* d = self->_d;
  size_t length = d->length;

  if (length == 0)
    return ERR_OK;

  if (d->reference.get() > 1)
  {
    FOG_RETURN_ON_ERROR(self->_detach());
    d = self->_d;
  }

  CharW* p = d->data;
  for (size_t i = 0; i < length; i++)
    p[i].bswap();

  d->hashCode = 0;
  return ERR_OK;
}

// ============================================================================
// [Fog::String - StringData]
// ============================================================================

template<typename CharT>
static CharT_(StringData)* FOG_CDECL StringT_dCreate(size_t capacity)
{
  if (capacity == 0)
    return StringT_getDEmpty<CharT>()->addRef();

  // Pad to 8/16 bytes.
  capacity = (capacity + 7) & ~7;

  size_t dsize = CharI_(StringData)::getSizeOf(capacity);
  CharT_(StringData)* d = reinterpret_cast<CharT_(StringData)*>(MemMgr::alloc(dsize));

  if (FOG_IS_NULL(d))
    return NULL;

  d->reference.init(1);
  d->vType = VarId<CharT_(String)>::ID | VAR_FLAG_NONE;
  d->hashCode = 0;

  d->capacity = capacity;
  d->length = 0;

  return d;
}

template<typename CharT, typename SrcT>
static CharT_(StringData)* FOG_CDECL StringT_dCreateStub(size_t capacity, const SrcT_(Stub)* stub)
{
  const SrcT* srcData = stub->getData();
  size_t srcLength = stub->getComputedLength();

  if (capacity < srcLength)
    capacity = srcLength;

  if (capacity == 0)
    return StringT_getDEmpty<CharT>()->addRef();

  // Pad to 8/16 bytes.
  capacity = (capacity + 7) & ~7;

  size_t dsize = CharI_(StringData)::getSizeOf(capacity);
  CharT_(StringData)* d = reinterpret_cast<CharT_(StringData)*>(MemMgr::alloc(dsize));

  if (FOG_IS_NULL(d))
    return NULL;

  d->reference.init(1);
  d->vType = VarId<CharT_(String)>::ID | VAR_FLAG_NONE;
  d->hashCode = 0;

  d->capacity = capacity;
  d->length = srcLength;

  StringT_chcopy(d->data, srcData, srcLength);
  d->data[srcLength] = 0;

  return d;
}

template<typename CharT>
static CharT_(StringData)* FOG_CDECL StringT_dAdopt(void* address, size_t capacity)
{
  if (capacity == 0)
    return StringT_getDEmpty<CharT>()->addRef();

  CharT_(StringData)* d = reinterpret_cast<CharT_(StringData)*>(address);

  d->reference.init(1);
  d->vType = VarId<CharT_(String)>::ID | VAR_FLAG_STATIC;
  d->hashCode = 0;
  d->capacity = capacity;
  d->length = 0;
  d->data[0] = 0;

  return d;
}

template<typename CharT, typename SrcT>
static CharT_(StringData)* FOG_CDECL StringT_dAdoptStub(void* address, size_t capacity, const SrcT_(Stub)* stub)
{
  const SrcT* srcData = stub->getData();
  size_t srcLength = stub->getComputedLength();

  if (srcLength > capacity)
  {
    SrcT_(Stub) stubModified(srcData, srcLength);
    return StringT_dCreateStub<CharT, SrcT>(srcLength, &stubModified);
  }
  else
  {
    CharT_(StringData)* d = reinterpret_cast<CharT_(StringData)*>(address);

    d->reference.init(1);
    d->vType = VarId<CharT_(String)>::ID | VAR_FLAG_STATIC;
    d->hashCode = 0;
    d->capacity = capacity;
    d->length = srcLength;
    StringT_chcopy(d->data, srcData, srcLength);
    d->data[srcLength] = 0;

    return d;
  }
}

template<typename CharT>
static CharT_(StringData)* FOG_CDECL StringT_dRealloc(CharT_(StringData)* d, size_t capacity)
{
  FOG_ASSERT(capacity >= d->length);

  size_t dsize = CharI_(StringData)::getSizeOf(capacity);
  if ((d->vType & VAR_FLAG_STATIC) == 0)
  {
    d = reinterpret_cast<CharT_(StringData)*>(MemMgr::realloc(d, dsize));
    if (FOG_IS_NULL(d))
      return NULL;

    d->capacity = capacity;
    return d;
  }
  else
  {
    CharT_(StringData)* newd = CharI_(String)::_dCreate(capacity, CharT_(Stub)(d->data, d->length));
    if (FOG_IS_NULL(newd))
      return NULL;

    d->release();
    return newd;
  }
}

template<typename CharT>
static void FOG_CDECL StringT_dFree(CharT_(StringData)* d)
{
  if ((d->vType & VAR_FLAG_STATIC) == 0)
    MemMgr::free(d);
}

// ============================================================================
// [Init / Fini]
// ============================================================================

FOG_NO_EXPORT void String_init(void)
{
  // --------------------------------------------------------------------------
  // [Funcs]
  // --------------------------------------------------------------------------

  fog_api.stringa_ctor = StringT_ctor<char>;
  fog_api.stringa_ctorCopyA = StringT_ctorCopy<char>;
  fog_api.stringa_ctorStubA = StringT_ctorStub<char, char>;
  fog_api.stringa_ctorStringAStringA = StringT_ctorStringString<char>;
  fog_api.stringa_ctorStringAStubA = StringT_ctorStringStub<char, char>;
  fog_api.stringa_ctorStubAStringA = StringT_ctorStubString<char, char>;
  fog_api.stringa_ctorStubAStubA = StringT_ctorStubStub<char, char>;
  fog_api.stringa_ctorSubstr = StringT_ctorSubstr<char>;
  fog_api.stringa_ctorU32 = StringT_ctorU32<char>;
  fog_api.stringa_ctorU64 = StringT_ctorU64<char>;
  fog_api.stringa_ctorDouble = StringT_ctorDouble<char>;
  fog_api.stringa_dtor = StringT_dtor<char>;

  fog_api.stringa_detach = StringT_detach<char>;
  fog_api.stringa_reserve = StringT_reserve<char>;
  fog_api.stringa_resize = StringT_resize<char>;
  fog_api.stringa_truncate = StringT_truncate<char>;
  fog_api.stringa_squeeze = StringT_squeeze<char>;
  fog_api.stringa_prepare = StringT_prepare<char>;
  fog_api.stringa_add = StringT_add<char>;

  fog_api.stringa_clear = StringT_clear<char>;
  fog_api.stringa_reset = StringT_reset<char>;

  fog_api.stringa_getHashCode = StringT_getHashCode<char>;

  fog_api.stringa_setStubA = StringT_setStub<char, char>;
  fog_api.stringa_setStringA = StringT_setString<char, char>;
  fog_api.stringa_setStringExA = StringT_setStringEx<char, char>;

  fog_api.stringa_setDeep = StringT_setDeep<char>;

  fog_api.stringa_appendStubA = StringT_appendStub<char, char>;
  fog_api.stringa_appendStringA = StringT_appendString<char, char>;
  fog_api.stringa_appendStringExA = StringT_appendStringEx<char, char>;

  fog_api.stringa_opFill = StringT_opFill<char>;
  fog_api.stringa_opBool = StringT_opBool<char>;
  fog_api.stringa_opI32 = StringT_opI32<char>;
  fog_api.stringa_opI32Ex = StringA_opI32Ex;
  fog_api.stringa_opU32 = StringT_opU32<char>;
  fog_api.stringa_opU32Ex = StringA_opU32Ex;
  fog_api.stringa_opI64 = StringT_opI64<char>;
  fog_api.stringa_opI64Ex = StringA_opI64Ex;
  fog_api.stringa_opU64 = StringT_opU64<char>;
  fog_api.stringa_opU64Ex = StringA_opU64Ex;
  fog_api.stringa_opDouble = StringA_opDouble;
  fog_api.stringa_opDoubleEx = StringA_opDoubleEx;

  fog_api.stringa_opVFormatStubA = StringA_opVFormatStubA;
  fog_api.stringa_opVFormatStringA = StringA_opVFormatStringA;

  fog_api.stringa_opZFormatStubA = StringT_opZFormatStub<char, char>;
  fog_api.stringa_opZFormatStringA = StringT_opZFormatString<char, char>;

  fog_api.stringa_opNormalizeSlashesA = StringT_opNormalizeSlashes<char>;

  fog_api.stringa_prependChars = StringT_prependChars<char>;
  fog_api.stringa_prependStubA = StringT_prependStub<char, char>;
  fog_api.stringa_prependStringA = StringT_prependString<char, char>;

  fog_api.stringa_insertChars = StringT_insertChars<char>;
  fog_api.stringa_insertStubA = StringT_insertStub<char, char>;
  fog_api.stringa_insertStringA = StringT_insertString<char, char>;

  fog_api.stringa_removeRange = StringT_removeRange<char>;
  fog_api.stringa_removeRangeList = StringT_removeRangeList<char>;

  fog_api.stringa_removeChar = StringT_removeChar<char>;
  fog_api.stringa_removeStubA = StringT_removeStub<char, char>;
  fog_api.stringa_removeStringA = StringT_removeString<char, char>;
  fog_api.stringa_removeRegExpA = StringT_removeRegExp<char>;

  fog_api.stringa_replaceRangeStubA = StringT_replaceRangeStub<char, char>;
  fog_api.stringa_replaceRangeStringA = StringT_replaceRangeString<char, char>;

  fog_api.stringa_replaceRangeListStubA = StringT_replaceRangeListStub<char, char>;
  fog_api.stringa_replaceRangeListStringA = StringT_replaceRangeListString<char, char>;

  fog_api.stringa_replaceChar = StringT_replaceChar<char>;

  fog_api.stringa_replaceStringA = StringT_replaceString<char>;
  fog_api.stringa_replaceRegExpA = StringT_replaceRegExp<char>;

  fog_api.stringa_lower = StringT_lower<char>;
  fog_api.stringa_upper = StringT_upper<char>;

  fog_api.stringa_trim = StringT_trim<char>;
  fog_api.stringa_simplify = StringT_simplify<char>;
  fog_api.stringa_justify = StringT_justify<char>;

  fog_api.stringa_splitChar = StringT_splitChar<char>;
  fog_api.stringa_splitStringA = StringT_splitString<char>;
  fog_api.stringa_splitRegExpA = StringT_splitRegExp<char>;

  fog_api.stringa_slice = StringT_slice<char>;

  fog_api.stringa_joinChar = StringT_joinChar<char>;
  fog_api.stringa_joinStringA = StringT_joinString<char>;

  fog_api.stringa_parseBool = StringT_parseBool<char>;
  fog_api.stringa_parseI8 = StringT_parseI8<char>;
  fog_api.stringa_parseU8 = StringT_parseU8<char>;
  fog_api.stringa_parseI16 = StringT_parseI16<char>;
  fog_api.stringa_parseU16 = StringT_parseU16<char>;
  fog_api.stringa_parseI32 = StringT_parseI32<char>;
  fog_api.stringa_parseU32 = StringT_parseU32<char>;
  fog_api.stringa_parseI64 = StringT_parseI64<char>;
  fog_api.stringa_parseU64 = StringT_parseU64<char>;
  fog_api.stringa_parseFloat = StringT_parseFloat<char>;
  fog_api.stringa_parseDouble = StringT_parseDouble<char>;

  fog_api.stringa_countOfChar = StringT_countOfChar<char>;
  fog_api.stringa_countOfStubA = StringT_countOfStub<char, char>;
  fog_api.stringa_countOfStringA = StringT_countOfString<char, char>;
  fog_api.stringa_countOfRegExpA = StringT_countOfRegExp<char>;

  fog_api.stringa_indexOfChar = StringT_indexOfChar<char>;
  fog_api.stringa_indexOfStubA = StringT_indexOfStub<char, char>;
  fog_api.stringa_indexOfStringA = StringT_indexOfString<char, char>;
  fog_api.stringa_indexOfRegExpA = StringT_indexOfRegExp<char>;
  fog_api.stringa_indexOfAnyCharA = StringT_indexOfAnyChar<char>;

  fog_api.stringa_lastIndexOfChar = StringT_lastIndexOfChar<char>;
  fog_api.stringa_lastIndexOfStubA = StringT_lastIndexOfStub<char, char>;
  fog_api.stringa_lastIndexOfStringA = StringT_lastIndexOfString<char, char>;
  fog_api.stringa_lastIndexOfRegExpA = StringT_lastIndexOfRegExp<char>;
  fog_api.stringa_lastIndexOfAnyCharA = StringT_lastIndexOfAnyChar<char>;

  fog_api.stringa_startsWithChar = StringT_startsWithChar<char>;
  fog_api.stringa_startsWithStubA = StringT_startsWithStub<char, char>;
  fog_api.stringa_startsWithStringA = StringT_startsWithString<char, char>;
  fog_api.stringa_startsWithRegExpA = StringT_startsWithRegExp<char>;

  fog_api.stringa_endsWithChar = StringT_endsWithChar<char>;
  fog_api.stringa_endsWithStubA = StringT_endsWithStub<char, char>;
  fog_api.stringa_endsWithStringA = StringT_endsWithString<char, char>;
  fog_api.stringa_endsWithRegExpA = StringT_endsWithRegExp<char>;

  fog_api.stringa_eqStubA = StringT_eqStub<char, char>;
  fog_api.stringa_eqStringA = StringT_eqString<char, char>;

  fog_api.stringa_eqStubExA = StringT_eqStubEx<char, char>;
  fog_api.stringa_eqStringExA = StringT_eqStringEx<char, char>;

  fog_api.stringa_compareStubA = StringT_compareStub<char, char>;
  fog_api.stringa_compareStringA = StringT_compareString<char, char>;

  fog_api.stringa_compareStubExA = StringT_compareStubEx<char, char>;
  fog_api.stringa_compareStringExA = StringT_compareStringEx<char, char>;

  fog_api.stringa_validateUtf8 = StringA_validateUtf8;
  fog_api.stringa_getUcsLength = StringA_getUcsLength;

  fog_api.stringa_hexDecode = StringA_hexDecode;
  fog_api.stringa_hexEncode = StringA_hexEncode;

  fog_api.stringa_base64DecodeStringA = StringA_base64DecodeStringA;
  fog_api.stringa_base64DecodeStringW = StringA_base64DecodeStringW;
  fog_api.stringa_base64DecodeDataA = StringA_base64DecodeData<char>;
  fog_api.stringa_base64DecodeDataW = StringA_base64DecodeData<CharW>;
  fog_api.stringa_base64EncodeStringA = StringA_base64EncodeStringA;
  fog_api.stringa_base64EncodeDataA = StringT_base64EncodeDataA<char>;

  fog_api.stringa_dCreate = StringT_dCreate<char>;
  fog_api.stringa_dCreateStubA = StringT_dCreateStub<char, char>;
  fog_api.stringa_dAdopt = StringT_dAdopt<char>;
  fog_api.stringa_dAdoptStubA = StringT_dAdoptStub<char, char>;
  fog_api.stringa_dRealloc = StringT_dRealloc<char>;
  fog_api.stringa_dFree = StringT_dFree<char>;

  fog_api.stringw_ctor = StringT_ctor<CharW>;
  fog_api.stringw_ctorCopyW = StringT_ctorCopy<CharW>;
  fog_api.stringw_ctorStubA = StringT_ctorStub<CharW, char>;
  fog_api.stringw_ctorStubW = StringT_ctorStub<CharW, CharW>;
  fog_api.stringw_ctorCodec = StringW_ctorCodec;
  fog_api.stringw_ctorStringWStringW = StringT_ctorStringString<CharW>;
  fog_api.stringw_ctorStringWStubW = StringT_ctorStringStub<CharW, CharW>;
  fog_api.stringw_ctorStringWStubA = StringT_ctorStringStub<CharW, char>;
  fog_api.stringw_ctorStubWStringW = StringT_ctorStubString<CharW, CharW>;
  fog_api.stringw_ctorStubWStubW = StringT_ctorStubStub<CharW, CharW>;
  fog_api.stringw_ctorStubAStringW = StringT_ctorStubString<CharW, char>;
  fog_api.stringw_ctorStubAStubA = StringT_ctorStubStub<CharW, char>;
  fog_api.stringw_ctorSubstr = StringT_ctorSubstr<CharW>;
  fog_api.stringw_ctorU32 = StringT_ctorU32<CharW>;
  fog_api.stringw_ctorU64 = StringT_ctorU64<CharW>;
  fog_api.stringw_ctorDouble = StringT_ctorDouble<CharW>;
  fog_api.stringw_dtor = StringT_dtor<CharW>;

  fog_api.stringw_detach = StringT_detach<CharW>;
  fog_api.stringw_reserve = StringT_reserve<CharW>;
  fog_api.stringw_resize = StringT_resize<CharW>;
  fog_api.stringw_truncate = StringT_truncate<CharW>;
  fog_api.stringw_squeeze = StringT_squeeze<CharW>;
  fog_api.stringw_prepare = StringT_prepare<CharW>;
  fog_api.stringw_add = StringT_add<CharW>;

  fog_api.stringw_clear = StringT_clear<CharW>;
  fog_api.stringw_reset = StringT_reset<CharW>;

  fog_api.stringw_getHashCode = StringT_getHashCode<CharW>;

  fog_api.stringw_setStubA = StringW_setStubCodec;
  fog_api.stringw_setStubW = StringT_setStub<CharW, CharW>;
  fog_api.stringw_setStringA = StringW_setStringCodec;
  fog_api.stringw_setStringW = StringT_setString<CharW, CharW>;
  fog_api.stringw_setStringExW = StringT_setStringEx<CharW, CharW>;

  fog_api.stringw_setDeep = StringT_setDeep<CharW>;

  fog_api.stringw_appendStubA = StringW_appendStubCodec;
  fog_api.stringw_appendStubW = StringT_appendStub<CharW, CharW>;
  fog_api.stringw_appendStringA = StringW_appendStringCodec;
  fog_api.stringw_appendStringW = StringT_appendString<CharW, CharW>;
  fog_api.stringw_appendStringExW = StringT_appendStringEx<CharW, CharW>;

  fog_api.stringw_opFill = StringT_opFill<CharW>;
  fog_api.stringw_opBool = StringT_opBool<CharW>;
  fog_api.stringw_opI32 = StringT_opI32<CharW>;
  fog_api.stringw_opI32Ex = StringW_opI32Ex;
  fog_api.stringw_opU32 = StringT_opU32<CharW>;
  fog_api.stringw_opU32Ex = StringW_opU32Ex;
  fog_api.stringw_opI64 = StringT_opI64<CharW>;
  fog_api.stringw_opI64Ex = StringW_opI64Ex;
  fog_api.stringw_opU64 = StringT_opU64<CharW>;
  fog_api.stringw_opU64Ex = StringW_opU64Ex;
  fog_api.stringw_opDouble = StringW_opDouble;
  fog_api.stringw_opDoubleEx = StringW_opDoubleEx;

  fog_api.stringw_opVFormatStubA = StringW_opVFormatStubA;
  fog_api.stringw_opVFormatStubW = StringW_opVFormatStubW;
  fog_api.stringw_opVFormatStringW = StringW_opVFormatStringW;

  fog_api.stringw_opZFormatStubW = StringT_opZFormatStub<CharW, CharW>;
  fog_api.stringw_opZFormatStringW = StringT_opZFormatString<CharW, CharW>;

  fog_api.stringw_opNormalizeSlashesW = StringT_opNormalizeSlashes<CharW>;

  fog_api.stringw_prependChars = StringT_prependChars<CharW>;
  fog_api.stringw_prependStubA = StringW_prependStubCodec;
  fog_api.stringw_prependStubW = StringT_prependStub<CharW, CharW>;
  fog_api.stringw_prependStringA = StringW_prependStringCodec;
  fog_api.stringw_prependStringW = StringT_prependString<CharW, CharW>;

  fog_api.stringw_insertChars = StringT_insertChars<CharW>;
  fog_api.stringw_insertStubA = StringW_insertStubCodec;
  fog_api.stringw_insertStubW = StringT_insertStub<CharW, CharW>;
  fog_api.stringw_insertStringA = StringW_insertStringCodec;
  fog_api.stringw_insertStringW = StringT_insertString<CharW, CharW>;

  fog_api.stringw_removeRange = StringT_removeRange<CharW>;
  fog_api.stringw_removeRangeList = StringT_removeRangeList<CharW>;

  fog_api.stringw_removeChar = StringT_removeChar<CharW>;
  fog_api.stringw_removeStubA = StringT_removeStub<CharW, char>;
  fog_api.stringw_removeStubW = StringT_removeStub<CharW, CharW>;
  fog_api.stringw_removeStringW = StringT_removeString<CharW, CharW>;
  fog_api.stringw_removeRegExpW = StringT_removeRegExp<CharW>;

  fog_api.stringw_replaceRangeStubW = StringT_replaceRangeStub<CharW, CharW>;
  fog_api.stringw_replaceRangeStringW = StringT_replaceRangeString<CharW, CharW>;

  fog_api.stringw_replaceRangeListStubW = StringT_replaceRangeListStub<CharW, CharW>;
  fog_api.stringw_replaceRangeListStringW = StringT_replaceRangeListString<CharW, CharW>;

  fog_api.stringw_replaceChar = StringT_replaceChar<CharW>;
  fog_api.stringw_replaceStringW = StringT_replaceString<CharW>;
  fog_api.stringw_replaceRegExpW = StringT_replaceRegExp<CharW>;

  fog_api.stringw_lower = StringT_lower<CharW>;
  fog_api.stringw_upper = StringT_upper<CharW>;

  fog_api.stringw_trim = StringT_trim<CharW>;
  fog_api.stringw_simplify = StringT_simplify<CharW>;
  fog_api.stringw_justify = StringT_justify<CharW>;

  fog_api.stringw_splitChar = StringT_splitChar<CharW>;
  fog_api.stringw_splitStringW = StringT_splitString<CharW>;
  fog_api.stringw_splitRegExpW = StringT_splitRegExp<CharW>;

  fog_api.stringw_slice = StringT_slice<CharW>;

  fog_api.stringw_joinChar = StringT_joinChar<CharW>;
  fog_api.stringw_joinStringW = StringT_joinString<CharW>;

  fog_api.stringw_parseBool = StringT_parseBool<CharW>;
  fog_api.stringw_parseI8 = StringT_parseI8<CharW>;
  fog_api.stringw_parseU8 = StringT_parseU8<CharW>;
  fog_api.stringw_parseI16 = StringT_parseI16<CharW>;
  fog_api.stringw_parseU16 = StringT_parseU16<CharW>;
  fog_api.stringw_parseI32 = StringT_parseI32<CharW>;
  fog_api.stringw_parseU32 = StringT_parseU32<CharW>;
  fog_api.stringw_parseI64 = StringT_parseI64<CharW>;
  fog_api.stringw_parseU64 = StringT_parseU64<CharW>;
  fog_api.stringw_parseFloat = StringT_parseFloat<CharW>;
  fog_api.stringw_parseDouble = StringT_parseDouble<CharW>;

  fog_api.stringw_countOfChar = StringT_countOfChar<CharW>;
  fog_api.stringw_countOfStubA = StringT_countOfStub<CharW, char>;
  fog_api.stringw_countOfStubW = StringT_countOfStub<CharW, CharW>;
  fog_api.stringw_countOfStringW = StringT_countOfString<CharW, CharW>;
  fog_api.stringw_countOfRegExpW = StringT_countOfRegExp<CharW>;

  fog_api.stringw_indexOfChar = StringT_indexOfChar<CharW>;
  fog_api.stringw_indexOfStubA = StringT_indexOfStub<CharW, char>;
  fog_api.stringw_indexOfStubW = StringT_indexOfStub<CharW, CharW>;
  fog_api.stringw_indexOfStringW = StringT_indexOfString<CharW, CharW>;
  fog_api.stringw_indexOfRegExpW = StringT_indexOfRegExp<CharW>;
  fog_api.stringw_indexOfAnyCharW = StringT_indexOfAnyChar<CharW>;

  fog_api.stringw_lastIndexOfChar = StringT_lastIndexOfChar<CharW>;
  fog_api.stringw_lastIndexOfStubA = StringT_lastIndexOfStub<CharW, char>;
  fog_api.stringw_lastIndexOfStubW = StringT_lastIndexOfStub<CharW, CharW>;
  fog_api.stringw_lastIndexOfStringW = StringT_lastIndexOfString<CharW, CharW>;
  fog_api.stringw_lastIndexOfRegExpW = StringT_lastIndexOfRegExp<CharW>;
  fog_api.stringw_lastIndexOfAnyCharW = StringT_lastIndexOfAnyChar<CharW>;

  fog_api.stringw_startsWithChar = StringT_startsWithChar<CharW>;
  fog_api.stringw_startsWithStubA = StringT_startsWithStub<CharW, char>;
  fog_api.stringw_startsWithStubW = StringT_startsWithStub<CharW, CharW>;
  fog_api.stringw_startsWithStringW = StringT_startsWithString<CharW, CharW>;
  fog_api.stringw_startsWithRegExpW = StringT_startsWithRegExp<CharW>;

  fog_api.stringw_endsWithChar = StringT_endsWithChar<CharW>;
  fog_api.stringw_endsWithStubA = StringT_endsWithStub<CharW, char>;
  fog_api.stringw_endsWithStubW = StringT_endsWithStub<CharW, CharW>;
  fog_api.stringw_endsWithStringW = StringT_endsWithString<CharW, CharW>;
  fog_api.stringw_endsWithRegExpW = StringT_endsWithRegExp<CharW>;

  fog_api.stringw_eqStubA = StringT_eqStub<CharW, char>;
  fog_api.stringw_eqStubW = StringT_eqStub<CharW, CharW>;
  fog_api.stringw_eqStringW = StringT_eqString<CharW, CharW>;

  fog_api.stringw_eqStubExA = StringT_eqStubEx<CharW, char>;
  fog_api.stringw_eqStubExW = StringT_eqStubEx<CharW, CharW>;
  fog_api.stringw_eqStringExW = StringT_eqStringEx<CharW, CharW>;

  fog_api.stringw_compareStubA = StringT_compareStub<CharW, char>;
  fog_api.stringw_compareStubW = StringT_compareStub<CharW, CharW>;
  fog_api.stringw_compareStringW = StringT_compareString<CharW, CharW>;

  fog_api.stringw_compareStubExA = StringT_compareStubEx<CharW, char>;
  fog_api.stringw_compareStubExW = StringT_compareStubEx<CharW, CharW>;
  fog_api.stringw_compareStringExW = StringT_compareStringEx<CharW, CharW>;

  fog_api.stringw_validateUtf16 = StringW_validateUtf16;
  fog_api.stringw_isCanonical = StringW_isCanonical;
  fog_api.stringw_makeCanonical = StringW_makeCanonical;
  fog_api.stringw_isNormalized = StringW_isNormalized;
  fog_api.stringw_quickCheck = StringW_quickCheck;
  fog_api.stringw_normalize = StringW_normalize;
  fog_api.stringw_getUcsLength = StringW_getUcsLength;

  fog_api.stringw_base64EncodeStringA = StringW_base64EncodeStringA;
  fog_api.stringw_base64EncodeDataA = StringT_base64EncodeDataA<CharW>;

  fog_api.stringw_bswap = StringW_bswap;

  fog_api.stringw_dCreate = StringT_dCreate<CharW>;
  fog_api.stringw_dCreateStubA = StringT_dCreateStub<CharW, char>;
  fog_api.stringw_dCreateStubW = StringT_dCreateStub<CharW, CharW>;
  fog_api.stringw_dAdopt = StringT_dAdopt<CharW>;
  fog_api.stringw_dAdoptStubA = StringT_dAdoptStub<CharW, char>;
  fog_api.stringw_dAdoptStubW = StringT_dAdoptStub<CharW, CharW>;
  fog_api.stringw_dRealloc = StringT_dRealloc<CharW>;
  fog_api.stringw_dFree = StringT_dFree<CharW>;

  // --------------------------------------------------------------------------
  // [Data]
  // --------------------------------------------------------------------------

  StringDataA* da = &StringA_dEmpty;
  StringDataW* dw = &StringW_dEmpty;

  da->reference.init(1);
  da->vType = VAR_TYPE_STRING_A | VAR_FLAG_STRING_INTERNED;

  dw->reference.init(1);
  dw->vType = VAR_TYPE_STRING_W | VAR_FLAG_STRING_INTERNED;

  fog_api.stringa_oEmpty = StringA_oEmpty.initCustom1(da);
  fog_api.stringw_oEmpty = StringW_oEmpty.initCustom1(dw);
}

} // Fog namespace
