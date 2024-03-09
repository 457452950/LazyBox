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
class TestAction;
class PreAction;
class EndAction;

class TestEngine : public Instance<TestEngine> {
    friend class TestCase;
    friend class PreAction;
    friend class EndAction;

public:
    TestEngine() = default;
    ~TestEngine();

    void RunAllTest();

protected:
    void AddCase(TestCase *a_case);
    void AddPreAction(TestAction *action);
    void AddEndAction(TestAction *action);

    void Report();

private:
    using case_list = std::vector<TestCase *>;
    std::unordered_map<std::string, case_list> name_2_cases_;

    using action_list = std::vector<TestAction *>;
    action_list pre_actions_;
    action_list end_actions_;
};

} // namespace lbox::test

#endif // LAZYBOX_INCLUDE_LAZYBOX_TEST_TESTENGINE_HPP_
