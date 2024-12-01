#include "lazybox/logger/Logger.hpp"

namespace lbox {

bool Logger::CheckTag(const std::string &tag) const {
    if(!this->tags_)
        return true;

#if(__cplusplus > 201703L)
    return this->tags_->contains(tag);
#else
    return this->tags_->find(tag) != this->tags_->end();
#endif
}

LogLevel Logger::Level() const { return this->level_; }

void Logger::Write(LogLevel level, const std::string &tag, const std::string &message) {
    if(!active_.load(std::memory_order_acquire)) {
        return;
    }
    if(level < this->level_) {
        return;
    }
    if(!this->CheckTag(tag)) {
        return;
    }

    auto entry = LogEntry{level, message};
    if(std_writer_) {
        std_writer_->Commit(entry);
    }
    for(auto it : loggers_) {
        it->Commit(entry);
    }
}

Logger *Logger::SetSTDLogger(bool enable) {
    this->std_writer_ = std::make_shared<ConsoleWriter>();
    return this;
}

Logger *Logger::AddFileLogger(const std::filesystem::path &output_path, bool async) {
    std::shared_ptr<FileWriter> writer;
    if(async) {
        writer = std::make_shared<AsyncFileWriter>();
    } else {
        writer = std::make_shared<FileWriter>();
    }
    if(!writer->Open(output_path)) {
        return nullptr;
    }
    loggers_.push_back(writer);
    return this;
}

void Logger::Stop() {
    active_.store(false, std::memory_order_release);
    for(auto &it : loggers_) {
        it->Stop();
    }
}

Logger::Logger() { this->formatter_ = std::make_shared<DefaultLogFormatter>(); }

Logger *Logger::SetFormmater(std::shared_ptr<LogFormatter> formatter) {
    this->formatter_ = formatter;
    return this;
}

Logger *Logger::AddReporter(std::shared_ptr<LogReporter> reporter) {
    this->loggers_.push_back(reporter);
    return this;
}

} // namespace lbox