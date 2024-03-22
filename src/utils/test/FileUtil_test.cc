#include "src/utils/FileUtil.h"
#include <cassert>
#include <cstdio>
#include <cstring>
using namespace lhqvq;

int main() {
    {
        file::AppendFile f("./appendfile1.txt");
        const char str[] = "0123456789\nabcdefghijklmnopqrstuvwxyz\n";
        f.Append(str, ::strlen(str));
        f.Flush();
        assert(f.GetWriteBytes() == ::strlen(str));
    }
    printf("Done\n");
    return 0;
}