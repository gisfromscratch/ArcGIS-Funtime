#include "FileGdbWorkspace.h"

#include "GeosecureGeodatabase.h"

FileGdbWorkspace::FileGdbWorkspace(const QString &path, QObject *parent) :
    QObject(parent),
    _geodatabase(std::unique_ptr<FileGDBAPI::Geodatabase>(new FileGDBAPI::Geodatabase))
{
    if (!GeosecureGeodatabase::Try(FileGDBAPI::OpenGeodatabase(path.toStdWString(), *_geodatabase)))
    {
        throw std::logic_error("Open geodatabase failed!");
    }
}

FileGdbWorkspace::~FileGdbWorkspace()
{
    GeosecureGeodatabase::Try(FileGDBAPI::CloseGeodatabase(*_geodatabase));
}

FileGdbTable *FileGdbWorkspace::openTable(const QString &name)
{
    QMutexLocker lock(&_tableMutex);
    try
    {
        auto table = new FileGdbTable(name, _geodatabase.get(), this);
        _tables[name] = table;
        return table;
    }
    catch (const std::logic_error&)
    {
        return nullptr;
    }
}
