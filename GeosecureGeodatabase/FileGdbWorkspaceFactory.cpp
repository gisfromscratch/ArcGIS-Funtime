#include "FileGdbWorkspaceFactory.h"

#include "GeosecureGeodatabase.h"

FileGdbWorkspaceFactory::FileGdbWorkspaceFactory(QObject *parent) :
    QObject(parent)
{
}

FileGdbWorkspace *FileGdbWorkspaceFactory::openWorkspace(const QString &path)
{
    QMutexLocker lock(&_workspaceMutex);
    if (_workspaces.end() == _workspaces.find(path))
    {
        try
        {
            auto workspace = new FileGdbWorkspace(path, this);
            _workspaces[path] = workspace;
            return workspace;
        }
        catch (const std::logic_error&)
        {
            return nullptr;
        }
    }
    else
    {
        return _workspaces[path];
    }
}
