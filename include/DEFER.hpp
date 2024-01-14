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

#define STR_CONTACT(A, B) A##B
#define STR_CONTACT2(A, B) STR_CONTACT(A, B)

/**
 * @example
 * {                                                        \n
 *      int *pint = new int[10];                            \n
 *      DEFER( [pint]()->void{ delete[] pint; } );          \n
 * }
 */
#define DEFER(func) auto STR_CONTACT2(__temp_, __LINE__) = std::make_shared<wutils::DeferHelper>(func)

} // namespace lbox

#endif // LAZYBOX_INCLUDE_DEFER_HPP_
