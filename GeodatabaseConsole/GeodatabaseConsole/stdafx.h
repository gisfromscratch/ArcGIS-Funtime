// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <iostream>
#include <functional>
#include <future>
#include <stdio.h>
#include <string>
#include <tchar.h>

// File Geodatabase API
#ifdef _DEBUG
#pragma comment(lib, "FileGDBAPID.lib")
#endif
#include <FileGDBAPI.h>
#include <FileGDBCore.h>
#include <Geodatabase.h>

// Commons API
#ifdef _DEBUG
#pragma comment(lib, "GeosecureGeodatabaseCommons.lib")
#endif
#include <GeosecureGeodatabaseCommons.h>
#include <FileGdbWorkspaceFactory.h>
