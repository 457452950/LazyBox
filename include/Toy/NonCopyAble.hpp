#pragma once
#ifndef LAZYBOX_INCLUDE_TOY_NONCOPYABLE_HPP_
#define LAZYBOX_INCLUDE_TOY_NONCOPYABLE_HPP_

namespace lbox {

class NonCopyAble {
public:
    NonCopyAble(const NonCopyAble &)            = delete;
    NonCopyAble &operator=(const NonCopyAble &) = delete;
};

} // namespace lbox

#endif // LAZYBOX_INCLUDE_TOY_NONCOPYABLE_HPP_
