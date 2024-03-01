#pragma once
#ifndef LAZYBOX_INCLUDE_LAZYBOX_TEST_TEST_H_
#define LAZYBOX_INCLUDE_LAZYBOX_TEST_TEST_H_

#include "TestEngine.hpp"
#include "TestCase.hpp"

#define TCASE(CASE_NAME, PART_NAME)                                                                                    \
    namespace {                                                                                                        \
    class TEST##CASE_NAME##PART_NAME : public lbox::test::TestCase {                                                   \
    public:                                                                                                            \
        TEST##CASE_NAME##PART_NAME() : TestCase(#CASE_NAME, #PART_NAME) {}                                             \
        void TestProcess() override;                                                                                   \
    } TEST##CASE_NAME##PART_NAME##__{};                                                                                \
    }                                                                                                                  \
    void TEST##CASE_NAME##PART_NAME::TestProcess()


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

#define TASSERT(EXPECTED, message)                                                                                     \
    if(!(EXPECTED)) {                                                                                                  \
        this->ext_message = message;                                                                                   \
        throw lbox::test::throw_assert{};                                                                              \
    } else {                                                                                                           \
        AddPass(true);                                                                                                 \
    }

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

} // namespace
#endif

#endif // LAZYBOX_INCLUDE_LAZYBOX_TEST_TEST_H_
