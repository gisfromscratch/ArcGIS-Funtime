#include "TimeLayer.h"

#include <QDebug>

TimeLayer::TimeLayer(EsriRuntimeQt::GraphicsLayer *graphicsLayer, EsriRuntimeQt::TimeInfo &timeInfo, const QString &dateTimeFormat, QObject *parent) :
    QObject(parent),
    m_graphicsLayer(graphicsLayer),
    m_timeInfo(timeInfo),
    m_dateTimeFormat(dateTimeFormat),
    m_queriedTimeExtent(false)
{
}


EsriRuntimeQt::TimeExtent TimeLayer::timeExtent()
{
    if (m_queriedTimeExtent)
    {
        return m_timeExtent;
    }

    m_timeExtent = queryTimeExtent();
    m_queriedTimeExtent = true;
    return m_timeExtent;
}


EsriRuntimeQt::TimeExtent TimeLayer::queryTimeExtent()
{
    EsriRuntimeQt::TimeExtent timeExtent = EsriRuntimeQt::TimeExtent();
    timeExtent.setStartDate(QDateTime());
    timeExtent.setEndDate(QDateTime());
    // Mo 24. Nov 01:00:00 -4714
    QDateTime invalidDate = timeExtent.startDate();
    QString startTimeField = m_timeInfo.startTimeField();
    QString endTimeField = m_timeInfo.endTimeField();
    if (startTimeField.isEmpty() && endTimeField.isEmpty())
    {
        qWarning() << "No start or end time field defined!";
        return timeExtent;
    }

    QList<int> graphicIds = m_graphicsLayer->graphicIDs();
    foreach (int graphicId, graphicIds)
    {
        EsriRuntimeQt::Graphic graphic = m_graphicsLayer->graphic(graphicId);
        if (!startTimeField.isEmpty())
        {
            QDateTime startDate = QDateTime::fromString(graphic.attributeValue(startTimeField).toString(), m_dateTimeFormat);
            if (startDate.isValid())
            {
                if (invalidDate == timeExtent.startDate() || startDate < timeExtent.startDate())
                {
                    timeExtent.setStartDate(startDate);
                }

                // Setzen des Endes des Zeitintervalls
                if (endTimeField.isEmpty())
                {
                    if (invalidDate == timeExtent.endDate() || timeExtent.endDate() < startDate)
                    {
                        timeExtent.setEndDate(startDate);
                    }
                }
            }
        }
        if (!endTimeField.isEmpty())
        {
            QDateTime endDate = QDateTime::fromString(graphic.attributeValue(endTimeField).toString(), m_dateTimeFormat);
            if (endDate.isValid())
            {
                if (invalidDate == timeExtent.endDate() || timeExtent.endDate() < endDate)
                {
                    timeExtent.setEndDate(endDate);
                }
            }
        }
    }
    return timeExtent;
}


EsriRuntimeQt::TimeInfo TimeLayer::timeInfo()
{
    return m_timeInfo;
}


EsriRuntimeQt::TimeExtent TimeLayer::timeInterval()
{
    return m_timeInterval;
}

void TimeLayer::setTimeInterval(EsriRuntimeQt::TimeExtent &timeInterval)
{
    m_timeInterval = timeInterval;
    QDateTime intervalStart = m_timeInterval.startDate();
    QDateTime intervalEnd = m_timeInterval.endDate();

    QString startTimeField = m_timeInfo.startTimeField();
    QString endTimeField = m_timeInfo.endTimeField();
    if (startTimeField.isEmpty() && endTimeField.isEmpty())
    {
        qWarning() << "Start and end time field are not set!";
        return;
    }

    QList<int> graphicIds = m_graphicsLayer->graphicIDs();
    foreach (int graphicId, graphicIds)
    {
        EsriRuntimeQt::Graphic graphic = m_graphicsLayer->graphic(graphicId);
        bool grapicHasValidTime = true;
        if (!startTimeField.isEmpty())
        {
            QDateTime startDateValue = QDateTime::fromString(graphic.attributeValue(startTimeField).toString(), m_dateTimeFormat);
            if (startDateValue.isValid())
            {
                if (startDateValue < intervalStart)
                {
                    grapicHasValidTime = false;
                }

                // Zusätzlich gegen das Ende des Zeitintervalls testen
                if (endTimeField.isEmpty() && intervalEnd < startDateValue)
                {
                    grapicHasValidTime = false;
                }
            }
            else
            {
                grapicHasValidTime = false;
            }
        }
        if (!endTimeField.isEmpty())
        {
            QDateTime endDateValue = QDateTime::fromString(graphic.attributeValue(endTimeField).toString(), m_dateTimeFormat);
            if (endDateValue.isValid())
            {
                if (intervalEnd < endDateValue)
                {
                    grapicHasValidTime = false;
                }
            }
            else
            {
                grapicHasValidTime = false;
            }
        }

        // TODO: Test, ob Grafik derzeit sichtbar ist
        m_graphicsLayer->setGraphicVisible(graphicId, grapicHasValidTime);
    }

    emit visibilityUpdated();
}
