#pragma once
#ifndef LAZYBOX_EXAMPLE_TEST_DEFER_HPP_
#define LAZYBOX_EXAMPLE_TEST_DEFER_HPP_

#include "lazybox/test/Test.h"
#include "lazybox/toy/DEFER.hpp"

TCASE(Defer, Defer) {
    int i = 0;
    {
        DEFER([&]() { i = 1; });
    }
    TASSERT(i == 1, "i == 1");

    auto ptr = new int(0);
    {
        DEFER([&]() {
            delete ptr;
            ptr = nullptr;
        });
    }
    TASSERT(ptr == nullptr, "ptr == nullptr");
}

#endif // LAZYBOX_EXAMPLE_TEST_DEFER_HPP_
