/**
 * lhqvq
 *
 *
 */

#include "src/utils/FileUtil.h"

#include <errno.h>
#include <cassert>
#include <cstring>
#include <cstdio>

using lhqvq::file::AppendFile;

AppendFile::AppendFile(const std::string &filename)
    : AppendFile(filename.c_str()) {}

AppendFile::AppendFile(const char *filename)
    : fp_(::fopen(filename, "a")),
      write_bytes_(0) {
        assert(fp_ != nullptr);
        ::setbuffer(fp_, buff_, sizeof(buff_));
}

void AppendFile::Append(const char *line, size_t len) {
    size_t remain = len;
    do {
        size_t n = ::fwrite_unlocked(line, 1, remain, fp_);
        if (n == 0) {
            int err = ::ferror(fp_);
            if (err) {
                char buf[512];
                fprintf(stderr, "AppendFile::Append() : %s\n", 
                        strerror_r(err, buf, sizeof(buf)));
            }
            write_bytes_ = len - remain;
            return;
        }
        remain -= n;
    } while (remain > 0);

    write_bytes_ += len;
}