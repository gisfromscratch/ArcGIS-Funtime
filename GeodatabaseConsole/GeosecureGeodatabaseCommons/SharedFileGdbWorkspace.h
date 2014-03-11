#pragma once

#include "GeosecureGeodatabaseCommons.h"

#include <map>
#include <mutex>

using namespace FileGDBAPI;

class GEOSECUREGEODATABASECOMMONS_API SharedFileGdbWorkspace : public FileGdbWorkspace
{
public:
	SharedFileGdbWorkspace(Geodatabase *geodatabase) : _geodatabase(geodatabase)
	{
	}

	~SharedFileGdbWorkspace()
	{
		lock_guard<mutex> lock(_tableMutex);
		for_each(_tables.begin(), _tables.end(), [&] (pair<wstring, Table*> tablePair) 
		{
			auto table = tablePair.second;
			_geodatabase->CloseTable(*table);
			delete table;
		});
		CloseGeodatabase(*_geodatabase);
		delete _geodatabase;
	}

	virtual Table *openTable(const wstring &tableName) override
	{
		//	lock_guard<mutex> lock(_tableMutex);
		auto tableIterator = _tables.find(tableName);
		if (_tables.end() == tableIterator)
		{
			// Add the table
			Table *table = new Table();
			if (Try(_geodatabase->OpenTable(tableName, *table)))
			{
				_tables[tableName] = table;
				return table;
			}
			else
			{
				return nullptr;
			}
		}
		else
		{
			return tableIterator->second;
		}
	}

private:
	Geodatabase *_geodatabase;

	map<wstring, Table*> _tables;
	mutex _tableMutex;
};

