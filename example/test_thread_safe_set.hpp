#pragma once
#ifndef LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_SET_HPP_
#define LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_SET_HPP_

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
}

#endif // LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_SET_HPP_
