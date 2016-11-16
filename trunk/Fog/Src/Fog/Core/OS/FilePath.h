// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_OS_FILEPATH_H
#define _FOG_CORE_OS_FILEPATH_H

// [Dependencies]
#include <Fog/Core/Global/Global.h>
#include <Fog/Core/Threading/Atomic.h>
#include <Fog/Core/Tools/Char.h>
#include <Fog/Core/Tools/String.h>

namespace Fog {

//! @addtogroup Fog_Core_OS
//! @{

// ============================================================================
// [Fog::FilePath]
// ============================================================================

struct FOG_NO_EXPORT FilePath
{
  // --------------------------------------------------------------------------
  // [Separators]
  // --------------------------------------------------------------------------

#if defined(FOG_OS_WINDOWS)
  static FOG_INLINE char getDirSeparatorA() { return '\\'; }
  static FOG_INLINE CharW getDirSeparatorW() { return CharW('\\'); }

  static FOG_INLINE char getPathSeparatorA() { return ';'; }
  static FOG_INLINE CharW getPathSeparatorW() { return CharW(';'); }
#endif // FOG_OS_WINDOWS

#if defined(FOG_OS_POSIX)
  static FOG_INLINE char getDirSeparatorA() { return '/'; }
  static FOG_INLINE CharW getDirSeparatorW() { return CharW('/'); }

  static FOG_INLINE char getPathSeparatorA() { return ':'; }
  static FOG_INLINE CharW getPathSeparatorW() { return CharW(':'); }
#endif // FOG_OS_POSIX

  static FOG_INLINE bool isDirSeparator(char c) { return c == getDirSeparatorA(); }
  static FOG_INLINE bool isDirSeparator(const CharW& c) { return c == getDirSeparatorW(); }

  static FOG_INLINE bool isAnyDirSeparator(char c) { return c == '/' || c == '\\'; }
  static FOG_INLINE bool isAnyDirSeparator(const CharW& c) { return c == CharW('/') || c == CharW('\\'); }

  static FOG_INLINE bool isPathSeparator(char c) { return c == getPathSeparatorA(); }
  static FOG_INLINE bool isPathSeparator(const CharW& c) { return c == getPathSeparatorW(); }

  // --------------------------------------------------------------------------
  // [Join]
  // --------------------------------------------------------------------------

  static FOG_INLINE err_t join(StringW& dst, const StubW& base, const StubW& part)
  {
    return fog_api.filepath_joinStubW(&dst, &base, &part);
  }

  static FOG_INLINE err_t join(StringW& dst, const StringW& base, const StringW& part)
  {
    return fog_api.filepath_joinStringW(&dst, &base, &part);
  }

  // --------------------------------------------------------------------------
  // [Extract]
  // --------------------------------------------------------------------------

  static FOG_INLINE err_t extractFile(StringW& dst, const StubW& path)
  {
    return fog_api.filepath_extractFileStubW(&dst, &path);
  }

  static FOG_INLINE err_t extractFile(StringW& dst, const StringW& path)
  {
    return fog_api.filepath_extractFileStringW(&dst, &path);
  }

  static FOG_INLINE err_t extractExtension(StringW& dst, const StubW& path)
  {
    return fog_api.filepath_extractExtensionStubW(&dst, &path);
  }

  static FOG_INLINE err_t extractExtension(StringW& dst, const StringW& path)
  {
    return fog_api.filepath_extractExtensionStringW(&dst, &path);
  }

  static FOG_INLINE err_t extractDirectory(StringW& dst, const StubW& path)
  {
    return fog_api.filepath_extractDirectoryStubW(&dst, &path);
  }

  static FOG_INLINE err_t extractDirectory(StringW& dst, const StringW& path)
  {
    return fog_api.filepath_extractDirectoryStringW(&dst, &path);
  }

  static FOG_INLINE bool containsFile(const StringW& path, const StubW& file, uint cs = CASE_SENSITIVE)
  {
    return fog_api.filepath_containsFileStubW(&path, &file, cs);
  }

  static FOG_INLINE bool containsFile(const StringW& path, const StringW& file, uint cs = CASE_SENSITIVE)
  {
    return fog_api.filepath_containsFileStringW(&path, &file, cs);
  }

  static FOG_INLINE bool containsExtension(const StringW& path, const StubW& extension, uint cs = CASE_SENSITIVE)
  {
    return fog_api.filepath_containsExtensionStubW(&path, &extension, cs);
  }

  static FOG_INLINE bool containsExtension(const StringW& path, const StringW& extension, uint cs = CASE_SENSITIVE)
  {
    return fog_api.filepath_containsExtensionStringW(&path, &extension, cs);
  }

  static FOG_INLINE bool containsDirectory(const StringW& path, const StubW& directory, uint cs = CASE_SENSITIVE)
  {
    return fog_api.filepath_containsDirectoryStubW(&path, &directory, cs);
  }

  static FOG_INLINE bool containsDirectory(const StringW& path, const StringW& directory, uint cs = CASE_SENSITIVE)
  {
    return fog_api.filepath_containsDirectoryStringW(&path, &directory, cs);
  }

  // --------------------------------------------------------------------------
  // [Normalize]
  // --------------------------------------------------------------------------

  static FOG_INLINE err_t normalize(StringW& dst, const StubW& path)
  {
    return fog_api.filepath_normalizeStubW(&dst, &path);
  }

  static FOG_INLINE err_t normalize(StringW& dst, const StringW& path)
  {
    return fog_api.filepath_normalizeStringW(&dst, &path);
  }

  static FOG_INLINE bool isNormalized(const StubW& path)
  {
    return fog_api.filepath_isNormalizedStubW(&path);
  }

  static FOG_INLINE bool isNormalized(const StringW& path)
  {
    return fog_api.filepath_isNormalizedStringW(&path);
  }

  // --------------------------------------------------------------------------
  // [Root]
  // --------------------------------------------------------------------------

  static FOG_INLINE bool isRoot(const StubW& path)
  {
    return fog_api.filepath_isRootStubW(&path);
  }

  static FOG_INLINE bool isRoot(const StringW& path)
  {
    return fog_api.filepath_isRootStringW(&path);
  }

  // --------------------------------------------------------------------------
  // [Relative / Absolute]
  // --------------------------------------------------------------------------

  static FOG_INLINE err_t toAbsolute(StringW& dst, const StubW& path)
  {
    return fog_api.filepath_toAbsoluteStubW(&dst, &path, NULL);
  }

  static FOG_INLINE err_t toAbsolute(StringW& dst, const StubW& path, const StubW& base)
  {
    return fog_api.filepath_toAbsoluteStubW(&dst, &path, &base);
  }

  static FOG_INLINE err_t toAbsolute(StringW& dst, const StringW& path)
  {
    return fog_api.filepath_toAbsoluteStringW(&dst, &path, NULL);
  }

  static FOG_INLINE err_t toAbsolute(StringW& dst, const StringW& path, const StringW& base)
  {
    return fog_api.filepath_toAbsoluteStringW(&dst, &path, &base);
  }

  static FOG_INLINE bool isAbsolute(const StubW& path)
  {
    return fog_api.filepath_isAbsoluteStubW(&path);
  }

  static FOG_INLINE bool isAbsolute(const StringW& path)
  {
    return fog_api.filepath_isAbsoluteStringW(&path);
  }

  // --------------------------------------------------------------------------
  // [Environment Substitution]
  // --------------------------------------------------------------------------

  //! @brief Substitute environment variables in @a path.
  //!
  //! @param dst Destination string where to write the substitution.
  //! @param path Source path which will be substituted.
  //! @param format Environment variable format, See @ref FILE_PATH_SUBSTITUTE_FORMAT.
  static FOG_INLINE err_t substituteEnvironmentVars(StringW& dst, const StringW& path,
    uint32_t format = FILE_PATH_SUBSTITUTE_FORMAT_DEFAULT)
  {
    return fog_api.filepath_substituteEnvironmentVars(&dst, &path, format);
  }
};

//! @}

} // Fog namespace

#endif // _FOG_CORE_OS_FILEPATH_H
