#pragma once

// Apple / BSD platform
#if defined(__APPLE__) || \
    defined(__FreeBSD__) || \
    defined(__NetBSD__) || \
    defined(__OpenBSD__)
  #define NETFIN_PLATFORM_APPLE     1
#else
  #define NETFIN_PLATFORM_APPLE     0
#endif

// Linux platform
#if defined(__linux__)
    #define NETFIN_PLATFORM_LINUX   1
#else
    #define NETFIN_PLATFORM_LINUX   0
#endif

// Windows platform
#if defined(_WIN32)
    #define NETFIN_PLATFORM_WINDOWS 1
#else
    #define NETFIN_PLATFORM_WINDOWS 0
#endif