#pragma once
#ifndef LAZYBOX_SRC_LOGGER_LOGWRITER_HPP_
#define LAZYBOX_SRC_LOGGER_LOGWRITER_HPP_

#include <fstream>
#include <future>
#include <filesystem>

#include "lazybox/logger/Basic.hpp"
#include "lazybox/thread_safe/Queue.hpp"
#include "lazybox/thread/Actor.hpp"

namespace lbox {

class LogReporter : public NonCopyAble {
public:
    LogReporter()          = default;
    virtual ~LogReporter() = default;

public:
    virtual void Commit(const LogEntry &entry) = 0;
    virtual void Stop()                        = 0;
};

class ConsoleWriter : public LogReporter {
public:
    ConsoleWriter()           = default;
    ~ConsoleWriter() override = default;

protected:
    void Commit(const LogEntry &entry) override;
    void Stop() override {}
};

class FileWriter : public LogReporter {
public:
    explicit FileWriter();
    ~FileWriter() override;

    bool Open(const std::filesystem::path &filename);

    void Stop() override;

    void Commit(const LogEntry &entry) override;

protected:
    int64_t GetFileSize();

    std::filesystem::path GetFilePath() const;

    void Flush();

    bool Reset(const std::filesystem::path &new_file);

protected:
    virtual void PreCommit(const LogEntry &entry);
    // flush in each commit
    virtual void Committed();

private:
    std::filesystem::path file_path_;
    //    std::filesystem::directory_entry file_entry_;
    std::ofstream        *output_{nullptr};
};

class AsyncFileWriter : public FileWriter, protected WaitableActor<LogEntry> {
public:
    AsyncFileWriter();
    ~AsyncFileWriter() override;

    void Stop() override;

    void Commit(const LogEntry &entry) override;

private:
    void work_loop();

    void stop();

private:
    std::atomic_bool  active_{true};
    std::future<void> stop_future_;
};


} // namespace lbox

#endif // LAZYBOX_SRC_LOGGER_LOGWRITER_HPP_
