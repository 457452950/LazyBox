#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_TEST_TESTCASE_HPP_
#define LAZYBOX_INCLUDE_LAZYBOX_TEST_TESTCASE_HPP_

#include <exception>
#include <string>

#include "../Chrono.h"

namespace lbox::test {


class throw_abort : std::exception {
public:
    const char *what() const noexcept override { return "throw_abort"; }
};

class throw_assert : std::exception {
public:
    const char *what() const noexcept override { return "throw_assert"; }
};

class throw_error : std::exception {
public:
    const char *what() const noexcept override { return "throw_error"; }
};

enum CaseState {
    // default
    CASE_STATE_UNKNOWN,
    // ok
    CASE_STATE_PASSED,
    // skip
    CASE_STATE_SKIPPED,
    // assert
    CASE_STATE_ASSERT,
    // error
    CASE_STATE_ERROR,
    // abort
    CASE_STATE_ABORTED,
    // throw
    CASE_STATE_THROW,

    // feature
    // CASE_STATE_TIMEOUT,
};

class TestCase {
public:
    std::string case_name;
    std::string part_name;

    int64_t time_start{0};
    int64_t time_end{0};

    CaseState   case_state{CASE_STATE_UNKNOWN};
    std::string ext_message;
    int32_t     total_count{0};
    int32_t     total_passed{0};

    void Run();

    void AddPass(bool t);

protected:
    virtual void TestProcess() = 0;

    explicit TestCase(const std::string &case_name, const std::string &part_name);
};

} // namespace lbox::test

#endif // LAZYBOX_INCLUDE_LAZYBOX_TEST_TESTCASE_HPP_
