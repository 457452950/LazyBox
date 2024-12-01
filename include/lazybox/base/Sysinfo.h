#pragma once
#ifndef LAZYBOX_INCLUDE_BASE_SYSINFO_H_
#define LAZYBOX_INCLUDE_BASE_SYSINFO_H_

#if defined(WIN32) || defined(_WIN32)
#define LBOX_WIN32 1
#elif defined(__linux__)
#define LBOX_LINUX 1
#endif


namespace lbox {}

#endif // LAZYBOX_INCLUDE_BASE_SYSINFO_H_
