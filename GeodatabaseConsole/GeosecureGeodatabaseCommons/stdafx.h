// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <iostream>
#include <map>
#include <functional>
#include <future>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include <vector>

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// File Geodatabase API
#ifdef _DEBUG
#pragma comment(lib, "FileGDBAPID.lib")
#endif
#include <FileGDBAPI.h>
#include <FileGDBCore.h>
#include <Geodatabase.h>
