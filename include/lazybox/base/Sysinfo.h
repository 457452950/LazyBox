#pragma once
#ifndef LAZYBOX_INCLUDE_BASE_SYSINFO_H_
#define LAZYBOX_INCLUDE_BASE_SYSINFO_H_

#ifdef WIN32
#define LBOX_WIN32
#elif defined(__linux__)
#define LBOX_LINUX
#endif


namespace lbox {}

#endif // LAZYBOX_INCLUDE_BASE_SYSINFO_H_
