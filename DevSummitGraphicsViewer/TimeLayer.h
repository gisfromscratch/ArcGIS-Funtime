#ifndef TIMELAYER_H
#define TIMELAYER_H

#include "TimeFieldInfo.h"

#include <GraphicsLayer.h>

#include <QObject>

class TimeLayer : public QObject
{
    Q_OBJECT
public:
    explicit TimeLayer(EsriRuntimeQt::GraphicsLayer *graphicsLayer, TimeFieldInfo *timeInfo, const QString &dateTimeFormat = "yyyy-MM-dd hh:mm:ss", QObject *parent = 0);

    EsriRuntimeQt::TimeExtent timeExtent();

    TimeFieldInfo *timeInfo();

    EsriRuntimeQt::TimeExtent timeInterval();
    void setTimeInterval(EsriRuntimeQt::TimeExtent &timeInterval);

signals:
    void visibilityUpdated();

public slots:

private:
    EsriRuntimeQt::TimeExtent queryTimeExtent();

    EsriRuntimeQt::GraphicsLayer *m_graphicsLayer;
    TimeFieldInfo *m_timeInfo;
    EsriRuntimeQt::TimeExtent m_timeInterval;

    EsriRuntimeQt::TimeExtent m_timeExtent;
    bool m_queriedTimeExtent;
    const QString m_dateTimeFormat;
};

#endif // TIMELAYER_H
