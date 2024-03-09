#include "lazybox/test/Test.h"

#include "test_defer.hpp"
#include "test_instance.hpp"
#include "test_thread_thread_pool.hpp"
#include "test_thread_actor.hpp"
#include "test_thread_channel.hpp"

#include "lazybox/logger/Logger.hpp"

TPREPARE {
    lbox::Logger::GetInstance()->SetConfig({lbox::LogLevel::L_INFO})->SetSTDLogger(true);

    LOG_INF("sys", "{}", __cplusplus);
};

TCLOSURE {
    LOG_INF("main", "test cases finished");
    lbox::Logger::GetInstance()->Stop();
};

int main() {
    lbox::test::TestEngine::GetInstance()->RunAllTest();
    return 0;
}
