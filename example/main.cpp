#include "lazybox/test/Test.h"
#include "lazybox/toy/NonCopyAble.hpp"

#include "test_defer.hpp"
#include "test_instance.hpp"
#include "test_thread_thread_pool.hpp"
#include "test_thread_actor.hpp"
#include "test_thread_channel.hpp"
#include "test_thread_safe_queue.hpp"
#include "test_thread_safe_set.hpp"
#include "test_thread_safe_map.hpp"
#include "test_random.hpp"

#include "lazybox/logger/Logger.hpp"

TPREPARE {
    lbox::Logger::GetInstance()->SetLevel(lbox::LogLevel::L_INFO)->SetSTDLogger(true)->AddFileLogger("test.log");

    LOG_INF("sys", "{}", __cplusplus);
    LOG_INF("test", "{}", "中国China完成");
};

TCLOSURE {
    LOG_INF("main", "test cases finished");
    lbox::Logger::GetInstance()->Stop();
    lbox::Logger::Destroy();
};

class NonCopyAbleClass {
public:
    NON_COPYABLE_(NonCopyAbleClass);
};

int main() {
    lbox::test::TestEngine::GetInstance()->RunAllTest();
    lbox::test::TestEngine::Destroy();
    return 0;
}
