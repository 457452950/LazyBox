#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_TEST_TEST_H_
#define LAZYBOX_INCLUDE_LAZYBOX_TEST_TEST_H_

#include "TestEngine.hpp"
#include "TestAction.hpp"
#include "TestCase.hpp"
#include "lazybox/toy/DEFER.hpp"
#include "lazybox/fmt/Format.h"

#define CANE_NAME this->case_name

#define TCASE(CASE_NAME_, PART_NAME)                                                                                   \
    namespace {                                                                                                        \
    class TEST##CASE_NAME_##PART_NAME : public lbox::test::TestCase {                                                  \
    public:                                                                                                            \
        TEST##CASE_NAME_##PART_NAME() : TestCase(#CASE_NAME_, #PART_NAME) {}                                           \
        void TestProcess() override;                                                                                   \
    } TEST##CASE_NAME_##PART_NAME##__{};                                                                               \
    }                                                                                                                  \
    void TEST##CASE_NAME_##PART_NAME::TestProcess()


#define TTRUE(EXPECTED, message)                                                                                       \
    AddPass((EXPECTED));                                                                                               \
    if(!(EXPECTED)) {                                                                                                  \
        this->case_state  = lbox::test::CaseState::CASE_STATE_ERROR;                                                   \
        this->ext_message = message;                                                                                   \
    }

#define TSKIP()                                                                                                        \
    {                                                                                                                  \
        this->case_state = lbox::test::CaseState::CASE_STATE_SKIPPED;                                                  \
        return;                                                                                                        \
    }

#define TASSERT(EXPECTED, ...)                                                                                         \
    if(!(EXPECTED)) {                                                                                                  \
        this->ext_message = lbox::format(__VA_ARGS__);                                                                 \
        throw lbox::test::throw_assert{};                                                                              \
    } else {                                                                                                           \
        AddPass(true);                                                                                                 \
    }

#define TPREPARE                                                                                                       \
    namespace {                                                                                                        \
    class SPLICE3(_TPREPARE, _, __LINE__) : public lbox::test::PreAction {                                             \
        void Do() override;                                                                                            \
    } SPLICE3(_TPREPARE, __COUNTER__, __LINE__);                                                                       \
    }                                                                                                                  \
    void SPLICE3(_TPREPARE, _, __LINE__)::Do()

#define TCLOSURE                                                                                                       \
    namespace {                                                                                                        \
    class SPLICE3(_TCLOSURE, _, __LINE__) : public lbox::test::EndAction {                                             \
        void Do() override;                                                                                            \
    } SPLICE3(_TCLOSURE, __COUNTER__, __LINE__);                                                                       \
    }                                                                                                                  \
    void SPLICE3(_TCLOSURE, _, __LINE__)::Do()


// #define TEST_DEMO
#ifdef TEST_DEMO
namespace {
TCASE(TEST_DEMO, EQUE) {
    printf("TEST_DEME_EQUE\n");

    TTRUE(1 == 1, "1 == 1");
    TTRUE(1 == 1, "1 == 1");
    TTRUE(1 == 1, "1 == 1");
}

TCASE(TEST_DEMO, FAILED) {
    printf("TEST_DEME_FAILED\n");

    TTRUE(1 == 1, "1 == 1");
    TTRUE(1 != 1, "1 != 1");
    TTRUE(2 == 1, "2 == 1");
}

TCASE(TEST_DEMO, SKIP) {
    printf("TEST_DEMO_SKIP\n");
    TSKIP();
}

TCASE(TEST_DEMO, ASSERT) {
    printf("TEST_DEMO_ASSERT\n");
    TASSERT(1 == 1, "1 == 1");
    TASSERT(1 != 1, "1 != 1");
}

TCASE(TEST_DEMO, THROW) {
    std::vector<int> a;
    a.at(3);
}

TCASE(TEST_DEMO, COST) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }

TPREPARE { printf("TPREPARE1\n"); }
TPREPARE { printf("TPREPARE2\n"); }

TCLOSURE { printf("TCLOSURE1\n"); };
TCLOSURE { printf("TCLOSURE2\n"); };

} // namespace
#endif

#endif // LAZYBOX_INCLUDE_LAZYBOX_TEST_TEST_H_
