#ifndef LAZYBOX_EXAMPLE_TEST_RANDOM_HPP_

#include "lazybox/test/Test.h"

#include <lazybox/random/Random.h>
#include <lazybox/logger/Logger.hpp>

TCASE(Random, Random) {
    lbox::RandomBox box{(unsigned)time(nullptr)};
    std::uniform_int_distribution<int> filter(0, 100);
    std::uniform_real_distribution<double> filter_double(0, 100);
    std::discrete_distribution<int> dist({40, 10, 10});


    for (auto i = 0; i < 40; ++i) {
        printf("  %u\n", box.Get());
        printf("    %ld\n", box.Filter(filter));
        printf("    %ld\n", box.Filter(std::uniform_int_distribution<int>{1, 100}));
        printf("    %f\n", box.Filter(filter_double));
        printf("    %d\n", box.Filter(dist));
    }


}

#endif // LAZYBOX_EXAMPLE_TEST_RANDOM_HPP_