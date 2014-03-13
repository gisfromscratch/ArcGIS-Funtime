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

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    QCoreApplication app(argc, argv);

    qDebug() << QCoreApplication::applicationDirPath();
    QFile jsonFile("AIS.json");
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        qWarning() << jsonFile.fileName() << "failed to open!";
        return -1;
    }

    QDateTime startTime = QDateTime::currentDateTime();
    const int ChunkSize = 1000;
    auto graphicCount = 0;

    QList<Geometry> geometries;
//    auto projectionEngine = SimpleProjectionEngine();
    auto projectionEngine = EsriProjectionEngine();
    QTextStream fileStream(&jsonFile);
    while (!fileStream.atEnd())
    {
        QString graphicAsJson = fileStream.readLine();
        auto graphic = Graphic();
        graphic.fromJson(graphicAsJson);

        // Project
        auto geometryAsWGS84 = projectionEngine.project(graphic.geometry(), WebMercator, WGS84);
        if (!geometryAsWGS84.isEmpty())
        {
            if (Geometry::isPoint(geometryAsWGS84.type()))
            {
                auto location = static_cast<Point>(geometryAsWGS84);
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
