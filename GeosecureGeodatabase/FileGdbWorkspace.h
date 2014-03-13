#ifndef FILEGDBWORKSPACE_H
#define FILEGDBWORKSPACE_H

#include "geosecuregeodatabase_global.h"
#include "FileGdbTable.h"

#include <Geodatabase.h>

#include <QMap>
#include <QMutex>
#include <QObject>

#include <memory>

class GEOSECUREGEODATABASESHARED_EXPORT FileGdbWorkspace : public QObject
{
    Q_OBJECT
public:
    explicit FileGdbWorkspace(const QString &path, QObject *parent = 0);
    ~FileGdbWorkspace();

    /*!
     * \brief openTable Opens the table
     * \param name      The name of the table.
     * \return          The opened table.
     */
    FileGdbTable *openTable(const QString &name);

signals:

public slots:

private:
    std::unique_ptr<FileGDBAPI::Geodatabase> _geodatabase;
    QMap<QString, FileGdbTable*> _tables;
    QMutex _tableMutex;
};

#endif // FILEGDBWORKSPACE_H
