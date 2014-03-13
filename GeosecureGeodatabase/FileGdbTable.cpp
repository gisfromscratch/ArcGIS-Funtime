#include "FileGdbTable.h"

#include "GeosecureGeodatabase.h"

#include <Point.h>

#include <Row.h>
#include <Table.h>

#include <QDebug>

FileGdbTable::FileGdbTable(const QString &name, FileGDBAPI::Geodatabase *geodatabase, QObject *parent) :
    _geodatabase(geodatabase),
    _table(std::unique_ptr<FileGDBAPI::Table>(new FileGDBAPI::Table)),
    QObject(parent)
{
    if (!GeosecureGeodatabase::Try(_geodatabase->OpenTable(name.toStdWString(), *_table)))
    {
        throw std::logic_error("Open table failed!");
    }
}

FileGdbTable::~FileGdbTable()
{
    GeosecureGeodatabase::Try(_geodatabase->CloseTable(*_table));
}

void FileGdbTable::insertGraphics(const QList<EsriRuntimeQt::Graphic> &graphics)
{
    using namespace FileGDBAPI;

    qDebug() << "Inserting" << graphics.size() << "features";

    Row row;

    EnumRows rows;
    _table->Search(L"OBJECTID", L"1=1", true, rows);
    while (S_OK == rows.Next(row))
    {
        _table->Delete(row);
    }

    _table->LoadOnlyMode(true);
    _table->SetWriteLock();
    _table->CreateRowObject(row);

    PointShapeBuffer buffer;
    Point *point;
    buffer.Setup(FileGDBAPI::shapePoint);
    buffer.GetPoint(point);
    foreach(EsriRuntimeQt::Graphic graphic, graphics)
    {
        EsriRuntimeQt::Geometry geometry = graphic.geometry();
        if (EsriRuntimeQt::Geometry::isPoint(geometry.type()))
        {
            EsriRuntimeQt::Point location = geometry;
            point->x = location.x();
            point->y = location.y();

            row.SetGeometry(buffer);
            _table->Insert(row);
        }
    }

    _table->LoadOnlyMode(false);
    _table->FreeWriteLock();

    qDebug() << graphics.size() << "features inserted";
}
