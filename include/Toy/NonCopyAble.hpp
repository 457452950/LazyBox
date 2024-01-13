#pragma once
#ifndef LAZYBOX_INCLUDE_TOY_NONCOPYABLE_HPP_
#define LAZYBOX_INCLUDE_TOY_NONCOPYABLE_HPP_

class NonCopyAble {
public:
    NonCopyAble(const NonCopyAble &)            = delete;
    NonCopyAble &operator=(const NonCopyAble &) = delete;
};

#endif // LAZYBOX_INCLUDE_TOY_NONCOPYABLE_HPP_
