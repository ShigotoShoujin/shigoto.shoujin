#pragma once
#include "../tstring.hpp"

namespace shoujin::file {

tstring GetProcessFile();
void FileDelete(tstring file);
bool DirectoryExists(tstring path);
bool FileExists(tstring file);
bool TryFileDelete(tstring file);

}
