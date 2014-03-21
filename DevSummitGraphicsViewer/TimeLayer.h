#ifndef TIMELAYER_H
#define TIMELAYER_H

#include "GraphicsCache.h"
#include "TimeFieldInfo.h"

#include <GraphicsLayer.h>

#include <QObject>

/*!
 * \brief Wraps a graphics layer and enables time restriction on the graphics.
 */
class TimeLayer : public QObject
{
    Q_OBJECT
public:
    explicit TimeLayer(EsriRuntimeQt::GraphicsLayer *graphicsLayer, TimeFieldInfo *timeInfo, const QString &dateTimeFormat = "yyyy-MM-dd hh:mm:ss", QObject *parent = 0);

    /*!
     * \brief timeExtent    The time extent of this layer.
     */
    EsriRuntimeQt::TimeExtent timeExtent();

    /*!
     * \brief timeInfo  Defines the start and end time field names.
     */
    TimeFieldInfo *timeInfo();

    /*!
     * \brief timeInterval  The time restriction for this layer.
     */
    EsriRuntimeQt::TimeExtent timeInterval();

    /*!
     * \brief setTimeInterval   Sets the time restriction for this layer.
     * \param timeInterval      The time restriction.
     */
    void setTimeInterval(EsriRuntimeQt::TimeExtent &timeInterval);

    /*!
     * \brief graphicsCache     The graphics cache associated with this layer.
     */
    GraphicsCache *graphicsCache();

    /*!
     * \brief setGrapicsCache   Sets the graphics cache for this layer.
     * \param graphicsCache     The graphics cache.
     */
    void setGrapicsCache(GraphicsCache *graphicsCache);

signals:
    void visibilityUpdated();

public slots:

private:
    EsriRuntimeQt::TimeExtent queryTimeExtent();

    EsriRuntimeQt::GraphicsLayer *m_graphicsLayer;
    TimeFieldInfo *m_timeInfo;
    EsriRuntimeQt::TimeExtent m_timeInterval;

    GraphicsCache *m_graphicsCache;

    EsriRuntimeQt::TimeExtent m_timeExtent;
    bool m_queriedTimeExtent;
    const QString m_dateTimeFormat;
};

#endif // TIMELAYER_H
