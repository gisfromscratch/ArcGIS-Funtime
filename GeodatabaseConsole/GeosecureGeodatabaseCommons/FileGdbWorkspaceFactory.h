#pragma once

#include "GeosecureGeodatabaseCommons.h"

#include "SharedFileGdbWorkspace.h"

#include <map>
#include <mutex>

class GEOSECUREGEODATABASECOMMONS_API FileGdbWorkspaceFactory
{
public:
	FileGdbWorkspaceFactory()
	{
	}
	
	virtual ~FileGdbWorkspaceFactory()
	{
		lock_guard<mutex> lock(_workspaceMutex);
		for_each(_workspaces.begin(), _workspaces.end(), [] (pair<wstring, FileGdbWorkspace*> workspacePair) 
		{
			delete workspacePair.second;
		});
	}

	/*!
	 * \brief openWorkspace	Opens the specified workspace.
	 * \param path			The path to the geodatabase.
	 * \return				The opened workspace.
	 */
	virtual FileGdbWorkspace *openWorkspace(const wstring &path)
	{
		lock_guard<mutex> lock(_workspaceMutex);
		auto workspaceIterator = _workspaces.find(path);
		if (_workspaces.end() == workspaceIterator)
		{
			// Add the new workspace
			Geodatabase *geodatabase = new Geodatabase();
			if (Try(OpenGeodatabase(path, *geodatabase)))
			{
				auto workspace = new SharedFileGdbWorkspace(geodatabase);
				_workspaces[path] = workspace;
				return workspace;
			}
			else
			{
				return nullptr;
			}
		}
		else
		{
			return workspaceIterator->second;
		}
	}

private:
	map<wstring, FileGdbWorkspace*> _workspaces;
	mutex _workspaceMutex;
};

