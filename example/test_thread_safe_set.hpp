#pragma once
#ifndef LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_SET_HPP_
#define LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_SET_HPP_

#include <unordered_set>

#include "lazybox/thread_safe/Set.hpp"
#include "lazybox/test/Test.h"

TCASE(ThreadSafe, Set) {
    {
        lbox::thread_safe::Set<std::string> s;

        std::string a = "asdads";
        s.Add(a);
        TASSERT(s.Contain(a), "set haven't {}", a);
        s.Remove(a);
        s.Add("asdads");
        TASSERT(s.Contain("asdads"), "set haven't {}", a);
        s.Remove("asdads");
    }
    {
        lbox::thread_safe::Set<int> s;

        int a = 1;
        s.Add(a);
        TASSERT(s.Contain(a), "set haven't {}", a);
        s.Remove(a);
        s.Add(1);
        TASSERT(s.Contain(1), "set haven't {}", a);
        s.Remove(1);
    }
    {
        lbox::thread_safe::Set<int> s;
        s.Add(1);
        s.Add(2);
        s.Add(3);
        s.Add(3);

        auto &&set = s.Dump();
        TASSERT(set == std::unordered_set<int>({1, 2, 3}), "set is not {{1, 2, 3}}");
    }
}

#endif // LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_SET_HPP_
