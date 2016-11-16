// [Fog-Core Library - Public API]
//
// [License]
// MIT, See COPYING file in package

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER

// [Precompiled Headers]
#if defined(FOG_PRECOMP)
#include FOG_PRECOMP
#endif // FOG_PRECOMP

// [Dependencies]
#include <Fog/Core/Assert.h>
#include <Fog/Core/Constants.h>
#include <Fog/Core/Std.h>

// ============================================================================
// [FOG_ASSERT]
// ============================================================================

FOG_CAPI_DECLARE void FOG_NO_RETURN fog_assert_failure(const char* file, int line, const char* msg)
{
  fog_stderr_msg(NULL, NULL, "Assertion failure: %s\nFile: %s\nLine: %d\n", msg ? msg :"", file, line);

  fog_failed = 1;
  exit(EXIT_FAILURE);
}
