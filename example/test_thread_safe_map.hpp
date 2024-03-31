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
    {
        lbox::thread_safe::Map<int, int> s;

        s.Add(std::pair{1, 1});
        s.Add(std::pair{1, 2});
        s.Add(std::pair{2, 2});
        s.Add(std::pair{3, 3});
        s.Add(std::pair{3, 3});
        s.Add(std::pair{4, 4});

        auto                       &&map = s.Dump();
        std::unordered_map<int, int> ans = {{1, 1}, {2, 2}, {3, 3}, {4, 4}};

        TASSERT(map == ans, "not equal");
    }
    {
        lbox::thread_safe::Map<int, char> s;

        s.Add(std::pair{1, 'a'});
        s.Add(std::pair{1, 'b'});
        s.Add(std::pair{2, 'b'});
        s.Add(std::pair{3, 'c'});
        s.Add(std::pair{3, 'c'});
        s.Add(std::pair{4, 'd'});

        auto                        &&map = s.Dump();
        std::unordered_map<int, char> ans = {{1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}};

        TASSERT(map == ans, "not equal");

        s.Foreach([](const std::pair<int, char> p) { printf("for each pair %d %c\n", p.first, p.second); });
    }
}

#endif // LAZYBOX_EXAMPLE_TEST_THREAD_SAFE_MAP_HPP_
