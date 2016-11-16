// [Fog-Core Library - Public API]
//
// [License]
// MIT, See COPYING file in package

// [Precompiled Headers]
#if defined(FOG_PRECOMP)
#include FOG_PRECOMP
#endif // FOG_PRECOMP

// [Dependencies]
#include <Fog/Core/Assert.h>
#include <Fog/Core/Atomic.h>
#include <Fog/Core/Byte.h>
#include <Fog/Core/ByteArrayMatcher.h>
#include <Fog/Core/StringUtil.h>
#include <Fog/Core/Thread.h>

namespace Fog {

// ============================================================================
// [Fog::ByteArrayMatcher]
// ============================================================================

static void buildTable(ByteArrayMatcher::SkipTable* skipTable, const char* ps, sysuint_t plen, uint cs)
{
  FOG_ASSERT(plen > 1);
  FOG_ASSERT(plen < UINT_MAX);

  if (skipTable->status.cmpXchg(ByteArrayMatcher::SkipTable::STATUS_NOT_INITIALIZED, ByteArrayMatcher::SkipTable::STATUS_INITIALIZING_NOW))
  {
    // Init skip table.
    sysuint_t a = 32; // 256 / 8
    sysuint_t i = plen;
    uint* data = skipTable->data;

    while (a--)
    {
      data[0] = (uint)i; data[1] = (uint)i; data[2] = (uint)i; data[3] = (uint)i;
      data[4] = (uint)i; data[5] = (uint)i; data[6] = (uint)i; data[7] = (uint)i;
      data += 8;
    }

    data = skipTable->data;

    if (cs == CASE_SENSITIVE)
    {
      while (i--)
      {
        data[(uint8_t)*ps] = (uint)i; ps++;
      }
    }
    else
    {
      while (i--)
      {
        data[(uint8_t)Byte::toLower(*ps)] = (uint)i; ps++;
      }
    }
    skipTable->status.set(ByteArrayMatcher::SkipTable::STATUS_INITIALIZED);
    return;
  }

  // Wait...another thread creating the table...
  while (skipTable->status.get() != ByteArrayMatcher::SkipTable::STATUS_INITIALIZED) Thread::_yield();
}

ByteArrayMatcher::ByteArrayMatcher()
{
  _skipTable[CASE_INSENSITIVE].status.init(SkipTable::STATUS_NOT_INITIALIZED);
  _skipTable[CASE_SENSITIVE  ].status.init(SkipTable::STATUS_NOT_INITIALIZED);
}

ByteArrayMatcher::ByteArrayMatcher(const ByteArray& pattern)
{
  setPattern(pattern);
}

ByteArrayMatcher::ByteArrayMatcher(const ByteArrayMatcher& matcher)
{
  setPattern(matcher);
}

ByteArrayMatcher::~ByteArrayMatcher()
{
}

err_t ByteArrayMatcher::setPattern(const ByteArray& pattern)
{
  err_t err;
  if ( (err = _pattern.set(pattern)) ) return err;

  // Mark tables as uninitialized.
  _skipTable[CASE_INSENSITIVE].status.set(SkipTable::STATUS_NOT_INITIALIZED);
  _skipTable[CASE_SENSITIVE  ].status.set(SkipTable::STATUS_NOT_INITIALIZED);

  return ERR_OK;
}

err_t ByteArrayMatcher::setPattern(const ByteArrayMatcher& matcher)
{
  SkipTable* dstSkipTable = _skipTable;
  const SkipTable* srcSkipTable = matcher._skipTable;

  err_t err;
  if ( (err = _pattern.set(matcher._pattern)) ) return err;

  // Copy skip tables if they are initialized.
  for (sysuint_t i = 0; i != 2; i++)
  {
    if (srcSkipTable[i].status.get() == SkipTable::STATUS_INITIALIZED)
    {
      dstSkipTable[i].status.set(SkipTable::STATUS_INITIALIZED);
      memcpy(&dstSkipTable[i].data, &srcSkipTable[i].data, sizeof(uint) * 256);
    }
    else
    {
      dstSkipTable[i].status.set(SkipTable::STATUS_NOT_INITIALIZED);
    }
  }

  return ERR_OK;
}

Range ByteArrayMatcher::match(const char* str, sysuint_t length, uint cs, const Range& range) const
{
  FOG_ASSERT(length != DETECT_LENGTH);
  FOG_ASSERT(range.index <= length);
  FOG_ASSERT(length - range.index >= range.length);

  sysuint_t patternLength = _pattern.getLength();

  // simple reject.
  if (patternLength == 0 || patternLength > length) return INVALID_INDEX;

  // We want 0 or 1.
  cs = !!cs;

  const uint8_t* strCur = reinterpret_cast<const uint8_t*>(str) + range.index;
  const uint8_t* patternStr = reinterpret_cast<const uint8_t*>(_pattern.getData());

  // Simple 'Char' search.
  if (patternLength == 1)
  {
    sysuint_t i = StringUtil::indexOf(reinterpret_cast<const char*>(strCur), range.length, (char)patternStr[0], cs);
    if (i != INVALID_INDEX) i += range.index;
    return i;
  }

  if (_skipTable[cs].status.get() != ByteArrayMatcher::SkipTable::STATUS_INITIALIZED)
  {
    buildTable(const_cast<SkipTable*>(&_skipTable[cs]), reinterpret_cast<const char*>(patternStr), patternLength, cs);
  }

  const uint* skipTable = _skipTable[cs].data;

  sysuint_t skip;
  sysuint_t remain = range.length;

  patternStr += patternLength - 1;
  strCur     += patternLength - 1;
  remain     -= patternLength - 1;

  if (cs == CASE_SENSITIVE)
  {
    for (;;)
    {
      // Get count of characters to skip from skip table.
      if ((skip = skipTable[*strCur]) == 0)
      {
        // Check if there is possible match.
        while (skip < patternLength)
        {
          if (strCur[-(sysint_t)skip] != patternStr[-(sysint_t)skip]) break;
          skip++;
        }

        // Match.
        if (skip >= patternLength)
          return Range((sysuint_t)(strCur - reinterpret_cast<const uint8_t*>(str)) - skip + 1, patternLength);

        if (skipTable[strCur[-(sysint_t)skip]] == patternLength)
          skip = patternLength - skip;
        else
          skip = 1;
      }

      if (remain < skip) break;
      strCur += skip;
      remain -= skip;
    }
  }
  else
  {
    for (;;)
    {
      // Get count of characters to skip from skip table.
      if ((skip = skipTable[Byte::toLower(*strCur)]) == 0)
      {
        // Check if there is possible match.
        while (skip < patternLength)
        {
          if (Byte::toLower(strCur[-(sysint_t)skip]) != Byte::toLower(patternStr[-(sysint_t)skip])) break;
          skip++;
        }

        // Match.
        if (skip >= patternLength)
          return Range((sysuint_t)(strCur - reinterpret_cast<const uint8_t*>(str)) - skip + 1, patternLength);

        if (skipTable[Byte::toLower(strCur[-(sysint_t)skip])] == patternLength)
          skip = patternLength - skip;
        else
          skip = 1;
      }

      if (remain < skip) break;
      strCur += skip;
      remain -= skip;
    }
  }

  return Range(INVALID_INDEX);
}

sysuint_t ByteArrayMatcher::getLength() const
{
  return _pattern.getLength();
}

} // Fog namespace
