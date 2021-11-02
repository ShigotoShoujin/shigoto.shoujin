#pragma once
#include "../tstring.hpp"

namespace shoujin::file {

class LogFile {
public:
#ifdef NDEBUG
	static tstring GetDebugFile()
	{
		return tstring();
	}

	static void AppendDebug(tstring text)
	{}

	static void AppendLineDebug(tstring text)
	{}
#else
	static tstring GetDebugFile();
	static void AppendDebug(tstring text);
	static void AppendLineDebug(tstring text);
#endif

	static void Append(std::string_view file, tstring text);
	static void AppendLine(std::string_view file, tstring text);
};

}
