set(utils_SRCS
  Timestamp.cpp
  FileUtil.cpp
)

add_library(lhqvq_utils OBJECT ${utils_SRCS})

add_subdirectory(tests)