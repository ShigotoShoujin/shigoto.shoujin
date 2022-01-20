#ifndef SHOUJIN_SOURCE_FILE_LOGFILE
#define SHOUJIN_SOURCE_FILE_LOGFILE

import Shoujin.String;
#include <string>

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
	/// <returns>%temp%\shoujin.log</returns>
	static tstring GetDebugFile();
	static void AppendDebug(tstring text);
	static void AppendLineDebug(tstring text);
#endif

	static void Append(tstring_view file, tstring text);
	static void AppendLine(tstring_view file, tstring text);
};

}

#endif
