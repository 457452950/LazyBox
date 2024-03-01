#include "lazybox/test/Test.h"

#include "test_defer.hpp"
#include "test_instance.hpp"
#include "test_thread_thread_pool.hpp"
#include "test_thread_actor.hpp"

int main() {

    lbox::test::TestEngine ::GetInstance()->RunAllTest();

    return 0;
}
