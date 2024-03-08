#include "lazybox/test/Test.h"

#include "test_defer.hpp"
#include "test_instance.hpp"
#include "test_thread_thread_pool.hpp"
#include "test_thread_actor.hpp"
#include "test_thread_channel.hpp"

#include "lazybox/Assert.hpp"
#include "lazybox/logger/Logger.hpp"

#include "lazybox/fmt/Format.h"
#include "lazybox/Chrono.h"

#include <filesystem>

int main() {
    auto path = std::filesystem::temp_directory_path();
    //    path      = path / "abasdasdc.log";
    //    path      = std::filesystem::current_path();
    path      = path / "abc.log";
    //    std::cout << path << std::endl;
    lbox::println("log file: {}", path);

    auto l = lbox::Logger::GetInstance()->SetConfig({lbox::LogLevel::L_INFO})->SetSTDLogger(true)->AddFileLogger(path);

    Assert(l, "errno {}", errno);
    std::chrono::time_point<std::chrono::system_clock> n = std::chrono::system_clock::now();

    lbox::DefaultLogFormatter f;
    lbox::Logger::GetInstance()->Write(
            lbox::LogLevel::L_DEBUG, "abc", f.Format(MAKELOGHEAD(lbox::LogLevel::L_DEBUG, "abc"), "asdasdasP{}asdsad"));


    LOG_INF("abd", "asdasd");
    LOG_INF("abd", "{}", "abasdas");
    LOG_INF("abd", "{}", n);
    LOG_DBG("abd", "{}", n);
    LOG_INF("abd", "{}", "abasdas");
    LOG_WAR("abd", "{}", "abasdas");
    LOG_ERR("abd", "{}", "abasdas");
    LOG_FAT("abd", "{}", "abasdas");

    lbox::Logger::GetInstance()->Stop();

    return 0;
}
