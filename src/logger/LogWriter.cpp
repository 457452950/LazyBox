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
    if(output_ && output_->is_open()) {
        output_->flush();
        output_->close();

        delete output_;
        output_ = nullptr;
    }
}

bool FileWriter::Open(const std::filesystem::path &filename) { return this->Reset(filename); }

void FileWriter::Commit(const LogEntry &entry) {
    this->PreCommit(entry);
    output_->write(entry.message.c_str(), static_cast<int64_t>(entry.message.size()));
    //    lbox::println("commit {} {}", entry.message, entry.message.size());
    this->Committed();
}

int64_t FileWriter::GetFileSize() {
    output_->seekp(0, std::ofstream::end);
    return output_->tellp();
}

void FileWriter::Committed() { this->Flush(); }

void FileWriter::Flush() {
    Assert(output_->is_open());
    output_->flush();
}

void FileWriter::PreCommit(const LogEntry &entry) {}

bool FileWriter::Reset(const std::filesystem::path &new_file) {
    if(file_path_ == new_file) {
        return true;
    }
    file_path_ = new_file;

    if(output_) {
        if(output_->is_open()) {
            output_->flush();
            output_->close();
        }
        delete output_;
        output_ = nullptr;
    }

    output_ = new std::ofstream(file_path_, std::ios::binary | std::ios::out | std::ios::app);
    return output_->is_open();
}

std::filesystem::path FileWriter::GetFilePath() const { return file_path_; }

void FileWriter::Stop() {
    if(output_->is_open()) {
        output_->flush();
        output_->close();
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

    this->Flush();
}

void AsyncFileWriter::stop() {
    if(active_) {
        active_.store(false);
        // wait for thread quit
        this->WakeUp();
        stop_future_.get();

        FileWriter::Stop();
    }
}


} // namespace lbox