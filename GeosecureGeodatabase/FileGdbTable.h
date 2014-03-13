#ifndef FILEGDBTABLE_H
#define FILEGDBTABLE_H

#include "geosecuregeodatabase_global.h"

#include <Geodatabase.h>
#include <Table.h>

#include <Graphic.h>

#include <QList>
#include <QObject>

#include <memory>

class GEOSECUREGEODATABASESHARED_EXPORT FileGdbTable : public QObject
{
    Q_OBJECT
public:
    explicit FileGdbTable(const QString &name, FileGDBAPI::Geodatabase *geodatabase, QObject *parent = 0);
    ~FileGdbTable();

    /*!
     * \brief insertGraphics    Inserts graphics into this table.
     * \param graphics          The graphics to be inserted.
     */
    void insertGraphics(const QList<EsriRuntimeQt::Graphic> &graphics);

signals:

public slots:

private:
    FileGDBAPI::Geodatabase *_geodatabase;
    std::unique_ptr<FileGDBAPI::Table> _table;
};

#endif // FILEGDBTABLE_H
