// SPDX-License-Identifier: MIT
//
// Copyright (c) 2020 Antonio Niño Díaz

#ifndef VERSION_H__
#define VERSION_H__

#include <ugba/ugba.h>

// This library follows the Semantic Versioning rules: https://semver.org/

#define LIBUGBA_VERSION_MAJOR   (0U)
#define LIBUGBA_VERSION_MINOR   (2U)
#define LIBUGBA_VERSION_PATCH   (0U)

#define LIBUGBA_VERSION         ((LIBUGBA_VERSION_MAJOR << 16) | \
                                 (LIBUGBA_VERSION_MINOR << 8) | \
                                 (LIBUGBA_VERSION_PATCH << 0))

// Returns the version of the shared library when it was compiled.
EXPORT_API uint32_t UGBA_VersionCompiledGet(void);

// This function can ve used to see if the version of the shared library is the
// same as the one that the program was compiled with. It isn't needed for the
// GBA version because it is linked statically.
//
// You should use the following right at the start of main() before UGBA_Init():
//
//  if (!UGBA_VersionIsCompatible(LIBUGBA_VERSION))
//      VersionCheckFailed();
//
//  It follows the rules of Semantic Versioning.
EXPORT_API int UGBA_VersionIsCompatible(uint32_t version);

#endif // VERSION_H__
