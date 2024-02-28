#pragma once
#ifndef LAZYBOX_INCLUDE_BASE_TYPETRAITS_HPP_
#define LAZYBOX_INCLUDE_BASE_TYPETRAITS_HPP_

#include <type_traits>

namespace lbox {

template <template <class...> class Target, class T>
struct is_template_of {
    static constexpr bool value = false;
};
template <template <class...> class Target, class... Args>
struct is_template_of<Target, Target<Args...>> {
    static constexpr bool value = true;
};

template <template <class...> class Target, class T>
inline constexpr bool is_template_of_v = false;

template <template <class...> class Target, class... Args>
inline constexpr bool is_template_of_v<Target, Target<Args...>> = true;

} // namespace lbox

#endif // LAZYBOX_INCLUDE_BASE_TYPETRAITS_HPP_
