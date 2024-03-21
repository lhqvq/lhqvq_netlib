/**
 * lhqvq
 *
 *
 */

#ifndef __FILEUTIL_H_
#define __FILEUTIL_H_

#include <string>

#include "src/utils/Uncopyable.h"

namespace lhqvq {
namespace file {

/**
 * @brief 只写文件包装类，以 append 方式
*/
class AppendFile : public Uncopyable {
public:
    explicit AppendFile(const std::string &filename);
    explicit AppendFile(const char *filename);
    ~AppendFile();

    /**
     * @brief 写数据
    */
    void Append(const char *line, size_t len);
    
    /**
     * @brief 刷新流缓冲区
    */
    void Flush();


    size_t GetWriteBytes() const { return write_bytes_; }

private:
    FILE *fp_;
    size_t write_bytes_;
    char buff_[64 * 1024]; // 64 KB 自定义，给 write_unlocked 使用
};


} // namespace file
} // namespace lhqvq

#endif // __FILEUTIL_H_