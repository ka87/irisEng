#pragma once

/* The engine's version, its build stamp and the machine it was built
 * for -- the three things the banner at the top of the log reports.
 *
 * The version lives here and nowhere else, so a release is one edit.
 */

#define IRISENG_VERSION_MAJOR 0
#define IRISENG_VERSION_MINOR 1
#define IRISENG_VERSION_PATCH 0

/* Built from the three numbers above rather than written out beside
   them. The dance costs two macros of its own, and it buys the one
   thing a second spelling cannot have: there is nothing to drift. */
#define IRISENG_STRINGISE_(x) #x
#define IRISENG_STRINGISE(x)  IRISENG_STRINGISE_(x)

#define IRISENG_VERSION_STRING                                         \
    IRISENG_STRINGISE(IRISENG_VERSION_MAJOR) "."                       \
    IRISENG_STRINGISE(IRISENG_VERSION_MINOR) "."                       \
    IRISENG_STRINGISE(IRISENG_VERSION_PATCH)

/* The architecture, from whatever the compiler in use says. MSVC and
 * the GCC-alikes name these differently and neither names them all, so
 * both are asked and anything unrecognised says so rather than
 * guessing: a wrong word in a log is worse than an honest unknown.
 */
#if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) || defined(__amd64__)
#define IRISENG_ARCH "x64"
#elif defined(_M_IX86) || defined(__i386__)
#define IRISENG_ARCH "x86"
#elif defined(_M_ARM64) || defined(__aarch64__)
#define IRISENG_ARCH "arm64"
#elif defined(_M_ARM) || defined(__arm__)
#define IRISENG_ARCH "arm"
#else
#define IRISENG_ARCH "unknown arch"
#endif

/* When this translation unit was compiled.
 *
 * It is the compile time of whichever file expands it, and that file is
 * dbg.c, which writes the banner. A build that recompiles only some of
 * the engine will report dbg.c's own date rather than the run's; after
 * a full build the two are the same thing.
 */
#define IRISENG_BUILD_STAMP __DATE__ " " __TIME__

/* The rule the banner is written between, and its middle line. Both
   are logged by DBG_LogBanner in dbg.c, which is the one place that
   expands the build stamp above. */
#define IRISENG_BANNER_RULE                                            \
    "================================================================"


#define IRISENG_BANNER_LINE                                            \
    "IRISEng v. " IRISENG_VERSION_STRING                               \
    ", built " IRISENG_BUILD_STAMP                                     \
    ", " IRISENG_ARCH
