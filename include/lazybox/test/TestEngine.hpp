#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_TEST_TESTENGINE_HPP_
#define LAZYBOX_INCLUDE_LAZYBOX_TEST_TESTENGINE_HPP_

#include <functional>
#include <vector>
#include <exception>

#include "../Chrono.h"
#include "../toy/Instance.hpp"

namespace lbox::test {

class TestCase;


class TestEngineImpl {
    friend class TestCase;

public:
    void RunAllTest();
    ~TestEngineImpl();

protected:
    void AddCase(TestCase *a_case);

    void Report();

private:
    using case_list = std::vector<TestCase *>;
    std::unordered_map<std::string, case_list> name_2_cases_;
};

using TestEngine = Instance<TestEngineImpl>;

} // namespace lbox::test

#endif // LAZYBOX_INCLUDE_LAZYBOX_TEST_TESTENGINE_HPP_
