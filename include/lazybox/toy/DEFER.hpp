#pragma once
#ifndef LAZYBOX_INCLUDE_DEFER_HPP_
#define LAZYBOX_INCLUDE_DEFER_HPP_

#include <functional>

namespace lbox {

class DeferHelper {
public:
    explicit DeferHelper(std::function<void()> f) : f_(std::move(f)){};
    //    explicit DeferHelper(std::function<void()> &&f) : f_(std::move(f)){};
    ~DeferHelper() { f_(); }
    std::function<void()> f_;
};

#define MAKEUNIQUENAME_(tag, A, B) tag##A##B
#define MAKEUNIQUENAME(tag, A, B) MAKEUNIQUENAME_(tag, A, B)

/**
 * @example
 * {                                                        \n
 *      int *pint = new int[10];                            \n
 *      DEFER( [pint]()->void{ delete[] pint; } );          \n
 * }
 */
#define DEFER(func) auto MAKEUNIQUENAME(_DEFER, __COUNTER__, __LINE__) = std::make_shared<lbox::DeferHelper>(func)

} // namespace lbox

#endif // LAZYBOX_INCLUDE_DEFER_HPP_
