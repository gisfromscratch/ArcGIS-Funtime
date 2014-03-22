#include <QCoreApplication>

#include "ProjectionEngine.h"
#include "EsriProjectionEngine.h"
#include "SimpleProjectionEngine.h"

#include <Geometry.h>
#include <Graphic.h>
#include <Point.h>

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QList>
#include <QTextStream>

using namespace EsriRuntimeQt;

static const SpatialReference WebMercator = SpatialReference(102100);
static const SpatialReference WGS84 = SpatialReference(4326);

static void projectGraphics(QTextStream &fileStream, const SpatialReference &sourceSpatialReference, const SpatialReference &targetSpatialReference, ProjectionEngine &projectionEngine)
{
    if (fileStream.atEnd())
    {
        // Allow reading multiple times
        fileStream.seek(0);
    }

    QDateTime startTime = QDateTime::currentDateTime();
    const int ChunkSize = 1000;
    auto graphicCount = 0;

    QList<Geometry> geometries;
    while (!fileStream.atEnd())
    {
        QString graphicAsJson = fileStream.readLine();
        auto graphic = Graphic();
        graphic.fromJson(graphicAsJson);

        // Project
        auto projectedGeometry = projectionEngine.project(graphic.geometry(), sourceSpatialReference, targetSpatialReference);
        if (!projectedGeometry.isEmpty())
        {
            if (Geometry::isPoint(projectedGeometry.type()))
            {
                auto location = static_cast<Point>(projectedGeometry);
                //qDebug() << location.x() << "," << location.y();
                geometries.append(location);
            }
        }

        if (0 == ++graphicCount % ChunkSize)
        {
            qDebug() << geometries.size() << "geometries projected [" << startTime.secsTo(QDateTime::currentDateTime()) << "secs ]";
        }
    }

    qDebug() << geometries.size() << "geometries projected [" << startTime.secsTo(QDateTime::currentDateTime()) << "secs ]";
}

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    QCoreApplication app(argc, argv);

    auto applicationPath = QCoreApplication::applicationDirPath();
    QFile jsonFile(applicationPath + "/../../Data/JSON/AIS.json");
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        qWarning() << jsonFile.fileName() << "failed to open!";
        return -1;
    }
    qDebug() << "Reading graphics from" << jsonFile.fileName();

    projectGraphics(QTextStream(&jsonFile), WebMercator, WGS84, EsriProjectionEngine());

    qDebug() << "Reading graphics from" << jsonFile.fileName();
    projectGraphics(QTextStream(&jsonFile), WebMercator, WGS84, SimpleProjectionEngine());
}
