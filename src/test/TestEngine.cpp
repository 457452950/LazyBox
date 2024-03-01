#include <iostream>
#include <algorithm>
#include "lazybox/test/TestEngine.hpp"

#include "lazybox/test/TestCase.hpp"

namespace lbox::test {

void TestEngineImpl::RunAllTest() {
    for(auto &[name, list] : name_2_cases_) {
        for(auto it : list) {
            it->Run();
        }
    }
    Report();
}

void TestEngineImpl::AddCase(TestCase *a_case) { this->name_2_cases_[a_case->case_name].push_back(a_case); }

TestEngineImpl::~TestEngineImpl() {}

void TestEngineImpl::Report() {
    printf("\n");
    std::for_each(this->name_2_cases_.begin(), this->name_2_cases_.end(), [this](auto &it) {
        auto &[name, list] = it;

        printf("---------------------------------------------------------\n");
        printf("|case : %s\n", name.c_str());

        int32_t total_cost = 0;

        std::for_each(list.begin(), list.end(), [&total_cost](TestCase *it) {
            switch(it->case_state) {
            case CASE_STATE_UNKNOWN:
                std::abort();
                // passed
            case CASE_STATE_PASSED:
                printf("|  part : %s, passed %d, result : success", it->part_name.c_str(), it->total_passed);
                break;

                // error
            case CASE_STATE_ERROR:
                printf("|  part : %s, passed %d/%d, result : failed",
                       it->part_name.c_str(),
                       it->total_passed,
                       it->total_count);
                break;

            case CASE_STATE_SKIPPED:
                printf("|  part : %s, result : skip", it->part_name.c_str());
                break;

                // assert
            case CASE_STATE_ASSERT:
                printf("|  part : %s, result : assert, message : %s", it->part_name.c_str(), it->ext_message.c_str());
                break;

            // abort
            case CASE_STATE_ABORTED:
                printf("|  part : %s, result : abort, message : %s", it->part_name.c_str(), it->ext_message.c_str());
                std::abort();

                // throw
            case CASE_STATE_THROW:
                printf("|  part : %s, result : throw, message : %s", it->part_name.c_str(), it->ext_message.c_str());
                break;
            }

            if(it->time_end >= it->time_start) {
                auto cost   = it->time_end - it->time_start;
                total_cost += cost;
                printf("                         -- cost : %lld ms\n", cost);
            } else {
                printf("\n");
            }
        });

        printf("|\n");
        printf("| total cost : %lld \n", total_cost);

        printf("---------------------------------------------------------\n");
    });
}

} // namespace lbox::test