// GeodatabaseConsole.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void OpenGeodatabase(const std::wstring &path, std::function<void (FileGDBAPI::Geodatabase&)> geodatabaseAction)
{
	using namespace FileGDBAPI;
	using namespace std;

	Geodatabase geodatabase;
	if (Try(OpenGeodatabase(path, geodatabase)))
	{
		wcout << path << L" was opened from thread " << this_thread::get_id() << endl;
		geodatabaseAction(geodatabase);
		Try(CloseGeodatabase(geodatabase));
	}
}

void OpenGeodatabaseShared(const std::wstring &path, std::function<void (FileGdbWorkspace*)> workspaceAction)
{
	using namespace FileGDBAPI;
	using namespace std;

	FileGdbWorkspaceFactory workspaceFactory;
	/*FileGdbWorkspace *workspace = workspaceFactory.openWorkspace(path);
	if (nullptr != workspace)
	{
		wcout << path << L" was opened from thread " << this_thread::get_id() << endl;
		workspaceAction(workspace);
	}*/
}

void OpenTable(FileGDBAPI::Geodatabase &geodatabase, const std::wstring &tableName)
{
	using namespace FileGDBAPI;
	using namespace std;

	Table table;
	if (Try(geodatabase.OpenTable(tableName, table)))
	{
		wcout << tableName << " was opened from thread " << this_thread::get_id() << endl;
		Try(geodatabase.CloseTable(table));
	}
}

void OpenTableShared(FileGdbWorkspace *workspace, const std::wstring &tableName)
{
	using namespace FileGDBAPI;
	using namespace std;

	/*Table *table = workspace->openTable(tableName);
	if (nullptr != table)
	{
		wcout << tableName << " was opened from thread " << this_thread::get_id() << endl;
	}*/
}

void OpenCitiesTable()
{
	using namespace FileGDBAPI;
	
	OpenGeodatabase(L"../data/Querying.gdb", [] (Geodatabase &geodatabase) -> void
	{
		OpenTable(geodatabase, L"Cities");
	});
}


int _tmain(int argc, _TCHAR* argv[])
{
	using namespace FileGDBAPI;
	using namespace std;

	const int TaskCount = 2;

	for (auto taskIndex = 0; taskIndex < TaskCount; taskIndex++)
	{
		auto openTask = async(OpenCitiesTable);
	}
	
	system("PAUSE");
	return 0;
}

