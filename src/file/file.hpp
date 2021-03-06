#ifndef SHOUJIN_SOURCE_FILE_FILE
#define SHOUJIN_SOURCE_FILE_FILE

import Shoujin.String;
#include <string>

namespace shoujin::file {

[[nodiscard]] tstring GetProcessFile();
void FileDelete(tstring file);
[[nodiscard]] bool DirectoryExists(tstring path);
[[nodiscard]] bool FileExists(tstring file);
[[nodiscard]] uintmax_t GetFileSize(tstring file);
[[nodiscard]] bool TryFileDelete(tstring file);

}

#endif
