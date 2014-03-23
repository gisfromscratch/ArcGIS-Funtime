#include "ShapefileWorkspaceFactory.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QUuid>
#include <WorkspaceInfo.h>
#include <WorkspaceInfoSet.h>

using namespace std;

ShapefileWorkspaceFactory::ShapefileWorkspaceFactory(QObject *parent) :
    QObject(parent),
    _basemapLayerIsValid(false)
{
    // Set the basemap layer for the local map service
    auto applicationPath = QCoreApplication::applicationDirPath();
    auto mapPackageFilePath = applicationPath + QDir::separator() + "Basemap.mpk";
    if (QFileInfo(mapPackageFilePath).exists())
    {
        _localMapService = LocalMapService(mapPackageFilePath);
        _localMapService.setEnableDynamicLayers(true);
        connect(&_localMapService, SIGNAL(serviceCreationSuccess(QString,QString)), this, SLOT(localServerStarted(QString,QString)));
        connect(&_localMapService, SIGNAL(serviceCreationFailure(QString)), this, SLOT(localServerFailed(QString)));
        _basemapLayerIsValid = true;
    }
    else
    {
        qWarning() << "Basemap layer" << mapPackageFilePath << "missing!";
    }
}

ShapefileWorkspaceFactory::~ShapefileWorkspaceFactory()
{
    disconnect(&_localMapService, SIGNAL(serviceCreationSuccess(QString,QString)), this, SLOT(localServerStarted(QString,QString)));
}

TableDataSource ShapefileWorkspaceFactory::openWorkspaceAsync(const QFileInfo &shapeFile)
{
    if (!_basemapLayerIsValid)
    {
        throw Exception("Local map service has no Basemap layer!");
    }

    if (!shapeFile.exists())
    {
        throw Exception("The shapefile does not exists!");
    }

    auto workspacePath = shapeFile.absolutePath();
    if (_openWorkspaces.contains(workspacePath))
    {
        // TODO: workspacesUpdated() signal must be emitted asynchronously!
        // Workspace was already opened
        TableDataSource dataSource;
        auto workspaceId = _openWorkspaces[workspacePath];
        dataSource.setWorkspaceId(workspaceId);
        dataSource.setDataSourceName(shapeFile.fileName());
        return dataSource;
    }

    // Adding workspaces requires the local map service being stopped
    if (_localMapService.isActive())
    {
        auto serviceStatus = _localMapService.stopAndWait();
        switch (serviceStatus)
        {
            case LocalServiceStatus::Stopped:
                break;
            default:
                throw Exception("Local map service could not be stopped!");
        }
    }

    // Create an unique datasource description
    TableDataSource dataSource;
    dataSource.setWorkspaceId(QUuid::createUuid().toString());
    dataSource.setDataSourceName(shapeFile.fileName());

    // Add the datasource as a shapefile workspace
    auto shapeFileWorkspace = WorkspaceInfo::CreateShapefileFolderConnection(dataSource.workspaceId(), workspacePath);
    shapeFileWorkspace.setEditable(true);
    WorkspaceInfoSet &workspaces = _localMapService.dynamicWorkspaces();
    workspaces.add(shapeFileWorkspace);

    // Add the shapefile workspace by using the path and the unique ID
    _openWorkspaces[workspacePath] = dataSource.workspaceId();

    // Start the local map service asynchronously
    _localMapService.start();
    return dataSource;
}

void ShapefileWorkspaceFactory::localServerStarted(const QString &, const QString &)
{
    // Local server was started
    auto mapServiceUrl = _localMapService.urlMapService();
    emit(workspacesUpdated(mapServiceUrl));
}

void ShapefileWorkspaceFactory::localServerFailed(const QString &errorMessage)
{
    qWarning() << "Local server start failed!";
    qWarning() << errorMessage;
}
