#pragma once
#ifndef LAZYBOX_INCLUDE_TOY_NONCOPYABLE_HPP_
#define LAZYBOX_INCLUDE_TOY_NONCOPYABLE_HPP_

namespace lbox {

class NonCopyAble {
public:
    NonCopyAble()                               = default;
    ~NonCopyAble()                              = default;
    NonCopyAble(const NonCopyAble &)            = delete;
    NonCopyAble &operator=(const NonCopyAble &) = delete;
};

#define NON_COPYABLE(CLASS)                                                                                            \
    CLASS(const CLASS &)            = delete;                                                                          \
    CLASS &operator=(const CLASS &) = delete;

} // namespace lbox

#endif // LAZYBOX_INCLUDE_TOY_NONCOPYABLE_HPP_
