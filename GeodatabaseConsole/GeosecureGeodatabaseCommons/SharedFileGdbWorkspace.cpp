#include "stdafx.h"

//
//SharedFileGdbWorkspace::SharedFileGdbWorkspace(Geodatabase *geodatabase) : _geodatabase(geodatabase)
//{
//}
//
//
//SharedFileGdbWorkspace::~SharedFileGdbWorkspace()
//{
//	lock_guard<mutex> lock(_tableMutex);
//	for_each(_tables.begin(), _tables.end(), [&] (const Table *table) 
//	{
//		_geodatabase->CloseTable(*table);
//		delete table;
//	});
//	CloseGeodatabase(*_geodatabase);
//	delete _geodatabase;
//}
//
//Table *SharedFileGdbWorkspace::openTable(const wstring &tableName)
//{
//	lock_guard<mutex> lock(_tableMutex);
//	auto tableIterator = _tables.find(tableName);
//	if (_tables.end() == tableIterator)
//	{
//		// Add the table
//		Table *table = new Table();
//		if (Try(_geodatabase.OpenTable(tableName, *table)))
//		{
//			_tables[tableName] = table;
//			return table;
//		}
//		else
//		{
//			return nullptr;
//		}
//	}
//	else
//	{
//		return tableIterator->second;
//	}
//}
