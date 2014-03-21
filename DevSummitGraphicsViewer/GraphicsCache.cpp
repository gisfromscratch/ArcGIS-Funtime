#include "GraphicsCache.h"

#include <QDebug>

GraphicsCache::GraphicsCache(QObject *parent) :
    _defaultGraphic(EsriRuntimeQt::Graphic()),
    QObject(parent)
{
}

void GraphicsCache::insert(int graphicId, const EsriRuntimeQt::Graphic &graphic)
{
    _graphics.insert(graphicId, graphic);
}

void GraphicsCache::insert(const QList<int> graphicIds, const QList<EsriRuntimeQt::Graphic> &graphics)
{
    if (graphicIds.size() != graphics.size())
    {
        qWarning() << "The sizes of the graphic IDs and the graphics does not match!";
        return;
    }

    auto graphicCount = graphics.size();
    qDebug() << "Inserting" << graphicCount << "graphics into the cache";
    for (auto index = 0; index < graphicCount; index++)
    {
        insert(graphicIds[index], graphics[index]);
    }
    qDebug() << "Cache was updated";
}

EsriRuntimeQt::Graphic GraphicsCache::value(int graphicId)
{
    if (_graphics.contains(graphicId))
    {
        return _graphics[graphicId];
    }

    qWarning() << graphicId << "is not managed by this graphics cache";
    return _defaultGraphic;
}
