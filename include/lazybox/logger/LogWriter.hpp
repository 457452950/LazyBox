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

class LogWriter : public NonCopyAble {
public:
    LogWriter()          = default;
    virtual ~LogWriter() = default;

public:
    virtual void Commit(const LogEntry &entry) = 0;
};

class ConsoleWriter : public LogWriter {
public:
    ConsoleWriter()           = default;
    ~ConsoleWriter() override = default;

protected:
    void Commit(const LogEntry &entry) override;
};

class FileWriter : public LogWriter {
public:
    explicit FileWriter();
    ~FileWriter() override;

    bool Open(const std::filesystem::path &filename);

    virtual void Stop();

    void Commit(const LogEntry &entry) override;

protected:
    int64_t GetFileSize();

    std::string GetFileName() const;

    void Flush();

    void Reset();

protected:
    virtual void PreCommit(const LogEntry &entry);
    // flush in each commit
    virtual void Committed();

private:
    std::string   file_name_;
    std::ofstream output_;
};

class AsyncFileWriter : public FileWriter, protected WaitableActor<LogEntry> {
public:
    AsyncFileWriter();
    ~AsyncFileWriter() override;

    void Stop() override;

protected:
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
