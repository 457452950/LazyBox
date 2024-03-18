#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_ASSERT_HPP_
#define LAZYBOX_INCLUDE_LAZYBOX_ASSERT_HPP_

#include <cassert>

#include "lazybox/base/CompileInfo.h"
#include "lazybox/fmt/Format.h"

#ifdef LBOX_DEBUG

#define PP_THIRD_ARG(a, b, c, ...) c
#define VA_OPT_SUPPORTED_I(...) PP_THIRD_ARG(__VA_OPT__(, ), true, false, )
#define VA_OPT_SUPPORTED VA_OPT_SUPPORTED_I(?)

#if(__cplusplus >= 202002L)
static_assert(VA_OPT_SUPPORTED);
#ifdef LBOX_WIN32
#define Assert(condition, ...)                                                                                         \
    if(!static_cast<bool>(condition)) {                                                                                \
        bool p = __VA_OPT__(!) false;                                                                                  \
        if(p) {                                                                                                        \
            lbox::println(stderr, "assert : " __VA_ARGS__);                                                            \
        } else {                                                                                                       \
            lbox::print(stderr, "assert : " #condition "\n");                                                          \
        }                                                                                                              \
        assert(condition);                                                                                             \
    }
#else
#define DISABLE
#define Assert(condition, ...)                                                                                         \
    if(!static_cast<bool>(condition)) {                                                                                \
        bool p = __VA_OPT__(!) false;                                                                                  \
        if(p) {                                                                                                        \
            lbox::print(stderr, fg(lbox::color::crimson), "assert : " __VA_ARGS__);                                    \
        } else {                                                                                                       \
            lbox::print(stderr, fg(lbox::color::crimson), "assert : " #condition "\n");                                \
        }                                                                                                              \
        fflush(stdout);                                                                                                \
        assert(condition);                                                                                             \
    }
#endif
#else

inline void Assert(bool condition) { assert(condition); }

template <class... Args>
void Assert(bool condition, const lbox::format_string<Args...> &fmt, Args... args) {
    if(!condition) {
#ifdef LBOX_WIN32
        lbox::print(stderr, "assert : {}\n", lbox::vformat(fmt, lbox::make_format_args(args...)));
#else
        lbox::print(
                stderr, fg(lbox::color::crimson), "assert : {}\n", lbox::vformat(fmt, lbox::make_format_args(args...)));
#endif
        assert(condition);
    }
}

#endif

#else

#define Assert(condition, ...) static_cast<void>(0)

#endif

namespace {
[[maybe_unused]] void _compile_check() {
    // #if(__cplusplus >= 202002L)
    Assert(1 == 2);
    Assert(1 == 1);
    Assert(1 != 2, "1 != 2");
    Assert(1 == 2, "1 != 2");
    Assert(1 == 2, "extra {} == {}\n", 1, 2);
    Assert(1 == 2, "extra {} == {} {}\n", 1, 2, std::this_thread::get_id());
    // #endif
}
} // namespace

#endif // LAZYBOX_INCLUDE_LAZYBOX_ASSERT_HPP_
