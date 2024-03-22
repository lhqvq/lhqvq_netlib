/**
 * lhqvq
 *
 *
 */

#include "src/logger/Logger.h"

#include <errno.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <utility>

#include "src/thread/CurrentThread.h"
#include "src/utils/Timestamp.h"

namespace lhqvq {

void DefaultOutput(const char* msg, size_t len) {
    fwrite(msg, sizeof(char), len, stdout);
}

void DefaultFlush() {
    fflush(stdout);
}

Logger::LogLevel gLogLevel = Logger::LogLevel::INFO;
Logger::OutputFunc gOutput = DefaultOutput;
Logger::FlushFunc gFlush = DefaultFlush;

const char* logLevelName[Logger::LogLevel::NUM_OF_LOGLEVEL] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL",
    "NONE"
};

} // namespace lhqvq

using lhqvq::Logger;
Logger::Logger(const char *file, int line, LogLevel level, int savedErrno)
    : stream_(), level_(level) {
    // TODO(GGGGITFKBJG): 将时间缓存, 减少Now()及ToString()的调用次数.
    stream_ << "[" << Timestamp::Now().ToString(true) << " ";
    stream_ << current_thread::Tid() << " ";
    stream_ << logLevelName[level] << " ";
    stream_ << file << ":" << line << "] ";
    if (savedErrno != 0) {
        // 用来存储errno对应的错误描述.
        char errbuf[512];
        const char *errstr = ::strerror_r(savedErrno, errbuf, sizeof(errbuf));
        stream_ << "(errno " << savedErrno << ": " << errstr << ") ";
    }
}

Logger::~Logger() {
    stream_ << "\n";
    std::string msg = std::move(stream_.str());
    gOutput(msg.c_str(), msg.size());
    if (level_ == LogLevel::FATAL) {
        gFlush();
        ::abort();
    }
}

Logger::LogLevel Logger::GetLogLevel() { return lhqvq::gLogLevel; }
void Logger::SetLogLevel(LogLevel level) { lhqvq::gLogLevel = level; }

void Logger::SetOutputFunc(OutputFunc output) { lhqvq::gOutput = output; }
void Logger::SetFlushFunc(FlushFunc flush) { lhqvq::gFlush = flush; }