#include "stdafx.h"

//FileGdbWorkspaceFactory::FileGdbWorkspaceFactory(void)
//{
//}
//
//FileGdbWorkspaceFactory::~FileGdbWorkspaceFactory(void)
//{
//	lock_guard<mutex> lock(_workspaceMutex);
//	for_each(_workspaces.begin(), _workspaces.end(), [] (const FileGdbWorkspace *workspace) 
//	{
//		delete workspace;
//	});
//}
//
//FileGdbWorkspace *FileGdbWorkspaceFactory::openWorkspace(const wstring &path)
//{
//	lock_guard<mutex> lock(_workspaceMutex);
//	auto workspaceIterator = _workspaces.find(path);
//	if (_workspaces.end() == workspaceIterator)
//	{
//		// Add the new workspace
//		Geodatabase *geodatabase = new Geodatabase();
//		if (Try(OpenGeodatabase(path, *geodatabase)))
//		{
//			auto workspace = new SharedFileGdbWorkspace(geodatabase);
//			_workspaces[path] = workspace;
//			return workspace;
//		}
//		else
//		{
//			return nullptr;
//		}
//	}
//	else
//	{
//		return nullptr;//workspaceIterator->second();
//	}
//}
