#include "lazybox/test/TestCase.hpp"

#include "lazybox/test/TestEngine.hpp"

namespace lbox::test {


void TestCase::Run() {
    try {
        time_start = GetTickMs64();
        this->TestProcess();
        time_end = GetTickMs64();

        if(case_state == CASE_STATE_SKIPPED) {
            return;
        }
        if(case_state == CASE_STATE_UNKNOWN)
            case_state = CASE_STATE_PASSED;

    } catch(throw_abort &e) {
        case_state = CASE_STATE_ABORTED;
    } catch(throw_assert &e) {
        case_state = CASE_STATE_ASSERT;
    } catch(throw_error &e) {
        case_state = CASE_STATE_ERROR;
    } catch(std::exception &e) {
        case_state  = CASE_STATE_THROW;
        ext_message = e.what();
        printf("%s\n", e.what());
    }
}

TestCase::TestCase(const std::string &case_name, const std::string &part_name) :
    case_name(case_name), part_name(part_name) {
    TestEngine::GetInstance()->AddCase(this);
}

void TestCase::AddPass(bool t) {
    if(t) {
        ++total_passed;
    }
    ++total_count;
}

} // namespace lbox::test