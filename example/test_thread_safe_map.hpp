#pragma once
#ifndef LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_MAP_HPP_
#define LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_MAP_HPP_

#include "lazybox/test/Test.h"
#include "lazybox/thread_safe/Map.hpp"

TCASE(ThreadSafe, Map) {
    {
        lbox::thread_safe::Map<std::string, int> s;
        int                                      ans = -1;

        std::string a = "asdads";
        s.Add(std::pair{a, 1});
        TASSERT(s.Contain(a), "set haven't {}", a);
        TASSERT(s.Get(a, ans), "map get {} fail", a);
        TASSERT(ans == 1, "ans {} not 1", ans);
        s.Remove(a);

        s.Add(std::pair{"asdads", 1});
        TASSERT(s.Contain("asdads"), "set haven't {}", a);
        TASSERT(s.Get("asdads", ans), "map get {} fail", a);
        TASSERT(ans == 1, "ans {} not 1", ans);
        s.Remove("asdads");
    }
}

#endif // LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_MAP_HPP_
