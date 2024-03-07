#include "lazybox/test/Test.h"

#include "test_defer.hpp"
#include "test_instance.hpp"
#include "test_thread_thread_pool.hpp"
#include "test_thread_actor.hpp"
#include "test_thread_channel.hpp"

#include "lazybox/Assert.hpp"
#include "lazybox/logger/Logger.hpp"

#include <filesystem>

int main() {
    auto path = std::filesystem::temp_directory_path();
    //    path      = path / "abasdasdc.log";
    //    path      = std::filesystem::current_path();
    path      = path / "abc.log";
    //    std::cout << path << std::endl;
    lbox::println("log file: {}", path);

    auto l = lbox::Logger::GetInstance()->SetConfig({lbox::LogLevel::L_DEBUG})->SetSTDLogger(true)->AddFileLogger(path);

    Assert(l, "errno {}", errno);

    lbox::Logger::GetInstance()->Write(
            lbox::LogLevel::L_DEBUG, "", lbox::format("hello world {}", std::this_thread::get_id()));
    lbox::Logger::GetInstance()->Write(
            lbox::LogLevel::L_DEBUG, "", lbox::format("hello world {}", std::this_thread::get_id()));
    lbox::Logger::GetInstance()->Write(
            lbox::LogLevel::L_DEBUG, "", lbox::format("hello world {}", std::this_thread::get_id()));
    lbox::Logger::GetInstance()->Write(
            lbox::LogLevel::L_DEBUG, "", lbox::format("hello world {}", std::this_thread::get_id()));

    //    Assert(1 == 2, "1!=2 {}", std::this_thread::get_id());
    //    lbox::test::TestEngine ::GetInstance()->RunAllTest();


    std::this_thread::sleep_for(std::chrono::seconds(1));

    lbox::Logger::GetInstance()->Stop();

    return 0;
}
