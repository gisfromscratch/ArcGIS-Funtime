#ifndef FILEGDBWORKSPACEFACTORY_H
#define FILEGDBWORKSPACEFACTORY_H

#include "geosecuregeodatabase_global.h"
#include "FileGdbWorkspace.h"

#include <Geodatabase.h>
#include <GeodatabaseManagement.h>

#include <QMap>
#include <QMutex>
#include <QObject>

class GEOSECUREGEODATABASESHARED_EXPORT FileGdbWorkspaceFactory : public QObject
{
    Q_OBJECT
public:
    explicit FileGdbWorkspaceFactory(QObject *parent = 0);

    /*!
     * \brief openWorkspace Opens the specified workspace.
     * \param path          The path to the geodatabase.
     * \return              The opened geodatabase instance.
     */
    FileGdbWorkspace *openWorkspace(const QString &path);

signals:

public slots:

private:
    QMap<QString, FileGdbWorkspace*> _workspaces;
    QMutex _workspaceMutex;
};

#endif // FILEGDBWORKSPACEFACTORY_H
