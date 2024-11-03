#include "lazybox/test/TestEngine.hpp"

#include "lazybox/test/TestCase.hpp"
#include "lazybox/test/TestAction.hpp"
#include "lazybox/Assert.hpp"

namespace lbox {
namespace test {

void TestEngine::RunAllTest() {
    for(auto action : this->pre_actions_) {
        action->Do();
    }

    for(auto &&item : name_2_cases_) {
        for(auto &&it : item.second) {
            it->Run();
        }
    }

    for(auto &&action : this->end_actions_) {
        action->Do();
    }

    Report();
}

void TestEngine::AddCase(TestCase *a_case) { this->name_2_cases_[a_case->case_name].push_back(a_case); }

TestEngine::~TestEngine() {}

void TestEngine::Report() {
    printf("\n");
    std::for_each(
            this->name_2_cases_.begin(),
            this->name_2_cases_.end(),
            [this](const std::pair<std::string, case_list> &it) {
                auto &&name = it.first;
                auto &&list = it.second;

                printf("---------------------------------------------------------\n");
                printf("|case : %s\n", name.c_str());

                int64_t total_cost = 0;

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
                        printf("|  part : %s, result : assert, message : %s",
                               it->part_name.c_str(),
                               it->ext_message.c_str());
                        break;

                    // abort
                    case CASE_STATE_ABORTED:
                        printf("|  part : %s, result : abort, message : %s",
                               it->part_name.c_str(),
                               it->ext_message.c_str());
                        std::abort();

                        // throw
                    case CASE_STATE_THROW:
                        printf("|  part : %s, result : throw, message : %s",
                               it->part_name.c_str(),
                               it->ext_message.c_str());
                        break;
                    }

                    if(it->time_end >= it->time_start) {
                        auto cost = it->time_end - it->time_start;
                        Assert(cost >= 0,
                               "test case cant be less than 0, case {}, part {}, cost {}",
                               it->case_name,
                               it->part_name,
                               cost);
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

void TestEngine::AddPreAction(TestAction *action) { this->pre_actions_.push_back(action); }

void TestEngine::AddEndAction(TestAction *action) { this->end_actions_.push_back(action); }

} // namespace test
} // namespace lbox