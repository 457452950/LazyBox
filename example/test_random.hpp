#ifndef LAZYBOX_EXAMPLE_TEST_RANDOM_HPP_

#include "lazybox/test/Test.h"

#include <lazybox/random/Random.h>
#include <lazybox/logger/Logger.hpp>

TCASE(Random, Random) {
    lbox::Random<> box{(unsigned)time(nullptr)};
    std::uniform_int_distribution<int> filter(0, 100);
    std::uniform_real_distribution<double> filter_double(0, 100);
    std::discrete_distribution<int> dist({40, 10, 10});


    for (auto i = 0; i < 40; ++i) {
        printf("  %u\n", box.Generate());
        printf("    %ld\n", box.Filter(filter));
        printf("    %ld\n", box.Filter(std::uniform_int_distribution<int>{1, 100}));
        printf("    %f\n", box.Filter(filter_double));
        printf("    %d\n", box.Filter(dist));
    }

    lbox::RandomBox<double> box1;
    auto set = box1.Generate(100, 0, 100.0);
    TASSERT(set.size() == 100, "set size is not 100 , but {}\n", set.size());
    for (auto&& i : set) {
        printf("  %f\n", i);
    }
    
    lbox::RandomBox<int> box2;
    auto set2 = box2.Generate(10, 0, 100);
    TASSERT(set2.size() == 10, "set size is not 100 , but {}\n", set2.size());
    for (auto&& i : set2) {
        printf("  %d\n", i);
    }
    printf("\n\n");
    set2 = box2.Generate(50, 0, 100);
    TASSERT(set2.size() == 50, "set size is not 50 , but {}\n", set2.size());
    for (auto&& i : set2) {
        printf("  %d\n", i);
    }
    printf("\n\n");
    set2 = box2.Generate(5, 0, 100);
    TASSERT(set2.size() == 5, "set size is not 5 , but {}\n", set2.size());
    for (auto&& i : set2) {
        printf("  %d\n", i);
    }
}

#endif // LAZYBOX_EXAMPLE_TEST_RANDOM_HPP_