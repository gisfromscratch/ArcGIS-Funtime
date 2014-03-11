#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GEOSECUREGEODATABASECOMMONS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GEOSECUREGEODATABASECOMMONS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef GEOSECUREGEODATABASECOMMONS_EXPORTS
#define GEOSECUREGEODATABASECOMMONS_API __declspec(dllexport)
#else
#define GEOSECUREGEODATABASECOMMONS_API __declspec(dllimport)
#endif

using namespace FileGDBAPI;
using namespace std;

#include <FileGDBAPI.h>
#include <FileGDBCore.h>

static bool Try(const fgdbError &error)
{
	using namespace std;

	if (S_OK == error)
	{
		return true;
	}
	else
	{
		wstring errorMessage;
		ErrorInfo::GetErrorDescription(error, errorMessage);
		wcerr << errorMessage << endl;
		return false;
	}
}

// This class is exported from the GeosecureGeodatabaseCommons.dll
class GEOSECUREGEODATABASECOMMONS_API FileGdbWorkspace {
public:
	FileGdbWorkspace();
	virtual ~FileGdbWorkspace();

	/*!
	 * \brief openTable	Opens the specified table.
	 * \param tableName	The name of the table to open.
	 * \return			The open table instance.
	 */
	virtual Table *openTable(const wstring &tableName) = 0;
};
