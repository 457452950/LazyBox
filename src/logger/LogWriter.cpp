#include "lazybox/logger/LogWriter.hpp"

#include <iostream>
#include <filesystem>

#include "lazybox/logger/Basic.hpp"

#include "lazybox/fmt/Format.h"
#include "lazybox/Assert.hpp"

namespace lbox {

void ConsoleWriter::Commit(const LogEntry &entry) {
    if(entry.level >= L_ERROR) {
        lbox::print(stderr, "{}\n", entry.message);
    } else {
        lbox::print(stdout, "{}\n", entry.message);
    }
}

FileWriter::FileWriter() = default;

FileWriter::~FileWriter() {
    if(output_.is_open()) {
        output_.flush();
        output_.close();
    }
}

bool FileWriter::Open(const std::filesystem::path &filename) {
    file_name_ = filename.string();
    lbox::println("file name {} {}", file_name_, filename);

    //    output_.clear();
    output_.open(file_name_, std::ios::binary | std::ios::out | std::ios::app);
    //    output_ << 1;
    GetFileSize();
    if(!output_.is_open()) {
        lbox::print(stderr, "Failed to open file {}\n", file_name_);
        return false;
    }
    return true;
}

void FileWriter::Commit(const LogEntry &entry) {
    this->PreCommit(entry);
    // 输出到文件
    //    output_.write(entry.message.c_str(), static_cast<int64_t>(entry.message.size()));
    //    output_ << 1;
    output_ << entry.message;
    //    lbox::println("commit {} {}", entry.message, entry.message.size());
    this->Committed();
}

int64_t FileWriter::GetFileSize() {
    output_.seekp(0, std::ofstream::end);
    return output_.tellp();
}

void FileWriter::Committed() {
    this->Flush();
    //    lbox::println("file size {}", GetFileSize());
}

void FileWriter::Flush() {
    Assert(output_.is_open());
    output_.flush();
}

void FileWriter::PreCommit(const LogEntry &entry) {}

void FileWriter::Reset() {
    if(output_.is_open()) {
        output_.flush();
        output_.close();
    }
}

std::string FileWriter::GetFileName() const { return file_name_; }

void FileWriter::Stop() {
    if(output_.is_open()) {
        output_.flush();
        output_.close();
    }
}


AsyncFileWriter::AsyncFileWriter() {
    this->stop_future_ = std::async(std::launch::async, &AsyncFileWriter::work_loop, this);
}

AsyncFileWriter::~AsyncFileWriter() { this->stop(); }

void AsyncFileWriter::Stop() { this->stop(); }

void AsyncFileWriter::Commit(const LogEntry &entry) { this->Send(entry); }

void AsyncFileWriter::work_loop() {
    while(active_) {
        LogEntry entry;

        if(this->Get(entry)) {
            FileWriter::Commit(entry);
        }
    }
}

void AsyncFileWriter::stop() {
    active_.store(false);
    // 等待工作线程退出
    this->WakeUp();
    stop_future_.get();

    FileWriter::Stop();
}


} // namespace lbox