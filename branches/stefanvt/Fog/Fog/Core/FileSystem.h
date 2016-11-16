// [Fog-Core Library - Public API]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_FILESYSTEM_H
#define _FOG_CORE_FILESYSTEM_H

// [Dependencies]
#include <Fog/Core/List.h>
#include <Fog/Core/String.h>

#if defined(FOG_OS_POSIX)
#include <sys/stat.h>
#endif // FOG_OS_POSIX

//! @addtogroup Fog_Core
//! @{

namespace Fog {
namespace FileSystem {

// ============================================================================
// [Fog::FileSystem]
// ============================================================================

enum FILE_FLAGS
{
  // [General Attributes]

  //! @brief Test if file element exist (it's a file, directory or anything else).
  FILE_EXISTS = (1 << 0),
  //! @brief Test if file is regular file.
  IS_FILE = (1 << 1),
  //! @brief Test if file is directory.
  IS_DIRECTORY = (1 << 2),
  //! @brief Test if file is symlink.
  IS_LINK = (1 << 3),
  //! @brief Test if file is executable.
  IS_EXECUTABLE = (1 << 4),
  //! @brief Test if file is hidden.
  IS_HIDDEN = (1 << 5),

  // [Windows Only Attributes]

  //! @brief File is archive, has Windows @c FILE_ATTRIBUTE_ARCHIVE attribute.
  IS_ARCHIVE = (1 << 6),
  //! @brief File is archive, has Windows @c FILE_ATTRIBUTE_COMPRESSED attribute.
  IS_COMPRESSED = (1 << 7),
  //! @brief File is archive, has Windows @c FILE_ATTRIBUTE_SPARSE_FILE attribute.
  IS_SPARSE = (1 << 8),
  //! @brief File is archive, has Windows @c FILE_ATTRIBUTE_SYSTEM attribute.
  IS_SYSTEM = (1 << 9),

  // [Permissions]

  //! @brief Test if file can be readed.
  CAN_READ = (1 << 5),
  //! @brief Test if file can be writed.
  CAN_WRITE = (1 << 6),
  //! @brief Test if file can be executed.
  CAN_EXECUTE = (1 << 7),

  // [All]

  //! @brief Test for all.
  FILE_ALL =
    FILE_EXISTS   |
    IS_FILE       |
    IS_DIRECTORY  |
    IS_LINK       |
    IS_EXECUTABLE |
    IS_HIDDEN     |

    IS_ARCHIVE    |
    IS_COMPRESSED |
    IS_SPARSE     |
    IS_SYSTEM     |

    CAN_READ      |
    CAN_WRITE     |
    CAN_EXECUTE
};

#if defined(FOG_OS_POSIX)
static int stat(const String& fileName, struct stat* s);
#endif // FOG_OS_POSIX

FOG_API uint32_t testFile(const String& fileName, uint32_t flags);
FOG_API bool findFile(const List<String>& paths, const String& fileName, String& dest);

static FOG_INLINE bool exists(const String& fileName)
{ return testFile(fileName, FILE_EXISTS) == FILE_EXISTS; }

static FOG_INLINE bool isFile(const String& fileName)
{ return testFile(fileName, IS_FILE) == IS_FILE; }

static FOG_INLINE bool isDirectory(const String& fileName)
{ return testFile(fileName, IS_DIRECTORY) == IS_DIRECTORY;}

FOG_API err_t createDirectory(const String& dir, bool recursive = true);
FOG_API err_t deleteDirectory(const String& dir);

} // FileSystem namespace
} // Fog namespace

//! @}

// [Guard]
#endif // _FOG_CORE_FILESYSTEM_H
